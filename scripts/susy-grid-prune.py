#!/usr/bin/env python2.7

"""
Script to submit prun jobs to skim D3PDs.

copies the `skimmer` directory into the current one
"""

import sys, os, re, shutil
import argparse
from subprocess import Popen, PIPE
from multiprocessing import Pool, Process, Queue
from distutils.spawn import find_executable
from ConfigParser import SafeConfigParser

# --- main submit function ---

def submit_ds(ds_name, debug=False, version=0, branches='branches.txt',
              out_talk=None, in_tar=None, maxgb=False, blacklist=[],
              skim_dir='skim-stuff', user=None):

    out_ds = _get_output_name(ds_name, version, user)

    bstring = 'cd {sd}; make clean; make; cd ..; ln {sd}/run-skim'.format(
        sd=skim_dir)
    run_string = 'run-skim %IN'

    input_args = [
        '--inDS=' + ds_name,
        '--outDS=' + out_ds,
        '--outputs=skim-output.root',
        '--excludeFile=*.tar,*.log,*.sh,*.out,*.root',
        '--extFile={}'.format(branches),
        '--rootVer=5.34/14',
        '--mergeOutput',
        '--cmtConfig=x86_64-slc6-gcc47-opt',
        ]
    if blacklist:
        input_args.append('--excludedSite={}'.format(','.join(blacklist)))
    if in_tar:
        input_args.append('--inTarBal={}'.format(in_tar))
    if maxgb:
        input_args.append('--nGBPerJob={}'.format(maxgb))

    submit_string = [
        'prun','--bexec', bstring,
        '--exec', run_string] + input_args
    if debug:
        submit_string = ['echo'] + submit_string
    ps = Popen(submit_string, stdout=PIPE, stderr=PIPE)
    out, err = ps.communicate()
    if out_talk:
        head_line = ' --- {} ---\n'.format(out_ds)
        out_talk.put(head_line + err + out + '\n')
    return out_ds, out, err

simfind = re.compile('\.e[0-9]{3,5}(_[sar][0-9]{3,5})+(_p[0-9]+)+')
def _get_tag(ds_name):
    try:
        ptag = re.compile('_p([0-9]{3,5})').findall(ds_name)[-1]
        simtag = simfind.search(ds_name)
        if simtag:
            return simtag.group(0).lstrip('.')
        return 'p' + ptag
    except IndexError:
        return None

def _get_output_name(ds_name, version, user=None):
    if not user:
        user = os.path.expandvars('$USER')
    preskim_name = _strip_front(ds_name)
    out_split = preskim_name.split('.')
    fmt_args = dict(
        in_ds='.'.join(out_split[:3]),
        rev=_get_tag(ds_name),
        version=version,
        user=user)
    output_tmp = 'user.{user}.{in_ds}.skim.{rev}_v{version}/'
    out_ds = output_tmp.format(**fmt_args)
    # grid won't accept names longer than 120 chars.
    # (actually, the stated error says the name must be less than 120 char)
    if len(out_ds) > 120:
        fmt_args['in_ds'] = '.'.join(out_split[:2])
        out_ds = output_tmp.format(**fmt_args)
    return out_ds


def _strip_front(ds_name):
    if not any(ds_name.startswith(x) for x in ['data','mc']):
        return _strip_front('.'.join(ds_name.split('.')[1:]))
    return ds_name

# --- helper classes ---

class LocalSkimmer(object):
    def __init__(self, branches, tarball=None, script_dir='skim-stuff',
                 user=None):
        self.vars_file = branches
        self.tarball = tarball
        self.script_dir = script_dir
        self.user = user or os.path.expandvars('$USER')
    def __enter__(self):
        if os.path.isdir(self.script_dir):
            shutil.rmtree(self.script_dir)
        self.link()
        if self.tarball:
            self.tar()
        self.current_files = set(os.listdir('.'))

    def link(self, clean=False):
        path_to_skim = os.path.join(os.path.dirname(__file__),'..','skim')
        abs_path = os.path.abspath(path_to_skim)
        if clean:
            try:
                run_args = dict(cwd=abs_path, shell=True, stdout=PIPE)
                Popen(['make clean'], **run_args).communicate()
            except OSError as err:
                print str(err)
        ignore = shutil.ignore_patterns(
            '*.root','*.txt','bin','dict','run-skim')
        shutil.copytree(abs_path, self.script_dir, ignore=ignore)

    def tar(self):
        user = self.user or os.path.expandvars('$USER')
        input_args = [
            '--exec="echo"',
            '--outDS=user.{}.nothing/'.format(user),
            '--excludeFile=*.tar,*.log,*.sh,*.out,*.root',
            '--extFile={}'.format(self.vars_file),
            '--outTarBall={}'.format(self.tarball),
            '--noSubmit',
            ]
        submit_string = ['prun'] + input_args
        ps = Popen(submit_string, stderr=PIPE, stdout=PIPE)
        out, err = ps.communicate()
        if 'ERROR' in err:
            raise IOError(
                'failure creating tarball for grid submit: {}'.format(err))

    def __exit__(self, ex_type, ex_val, trace):
        shutil.rmtree(self.script_dir)
        if self.tarball:
            os.remove(self.tarball)

        # clean up junk
        new_files = set(os.listdir('.')) - self.current_files
        for f in new_files:
            if os.path.isdir(f):
                shutil.rmtree(f)
            else:
                os.remove(f)

class Reporter(Process):
    """
    Prints output from a queue which it owns. The queue should be given to
    each subprocess which is submitting jobs.
    """
    completed_str = (
        'No jobs to be submitted since all input files were'
        ' (or are being) processed for the outDS.')

    def __init__(self, n_datasets, logfile=None):
        super(Reporter,self).__init__()
        self.queue = Queue()
        self.n_datasets = n_datasets
        self.logfile = logfile
        self.n_answer = 0
        self.n_error = 0
        self.n_already_done = 0
        self.output = sys.stdout
        def basic_error_search(x):
            return 'ERROR' in x
        self.has_error = basic_error_search
        def already_complete(x):
            return self.completed_str in x
        self.already_done = already_complete

    def _get_supp(self):
        supp = []
        if self.n_already_done > 0:
            donestr = '\033[32;1m{} already finished\033[m'.format(
                self.n_already_done)
            supp.append(donestr)
        if self.n_error > 0:
            errstr = '\033[31m{e} ERRORS\033[m'.format(e=self.n_error)
            supp.append(errstr)
        if not supp:
            return ''
        submits = self.n_answer - self.n_error - self.n_already_done
        supp.append('{} submitted'.format(submits))
        return ' ({})'.format(', '.join(supp))

    def _tty_out(self):
        out_template = '\rdone with {} of {}'
        outline = out_template.format(self.n_answer, self.n_datasets)
        outline += self._get_supp()
        self.output.write(outline)
        self.output.flush()
    def _file_out(self, message):
        if self.has_error(message):
            result = 'ERROR'
        elif self.already_done(message):
            result = 'DONE'
        else:
            result = 'SUCCESS'
        out = '{} of {}, result: {}\n'.format(
            self.n_answer, self.n_datasets, result)
        self.output.write(out)
        self.output.flush()

    def run(self):
        not_dead = True
        while not_dead:
            message = self.queue.get()
            if message.lower() == 'kill':
                not_dead = False
                continue
            else:
                self.n_answer += 1
                if self.has_error(message):
                    self.n_error += 1
                if self.already_done(message):
                    self.n_already_done += 1
                if self.output.isatty():
                    self._tty_out()
                else:
                    self._file_out(message)
            if self.logfile:
                self.logfile.write(message)
                self.logfile.flush()
        if self.output.isatty():
            self.output.write('\n')
    def close(self):
        """
        to be called externally
        """
        self.queue.put('kill')
        self.queue.close()
        self.join()

# --- various configuration ---

def _get_config(config_name):
    config = SafeConfigParser()
    config.read([config_name])
    sections = config.sections()
    vsec = 'version'
    ssec = 'site'

    if any(sec not in sections for sec in [vsec, ssec]):
        if not config.has_section(vsec):
            config.add_section(vsec)
            config.set(vsec,'version','0')
        if not config.has_section(ssec):
            config.add_section(ssec)
            config.set(ssec,'user', os.path.expandvars('$USER'))
        with open(config_name,'w') as cfg_file:
            config.write(cfg_file)

    return config

def _get_args():
    parser = argparse.ArgumentParser(description=__doc__)

    d = 'default: %(default)s'
    parser.add_argument('ds_list', nargs='?',
                        help='file listing datasets (can also pipe in list)')
    parser.add_argument('-v','--variable-list',
                        help='file containing variables to skim over')
    parser.add_argument('--mono', action='store_true',
                        help="don't multiprocess")
    parser.add_argument('-o', '--out-name', default='output-containers.txt',
                        help=d)
    parser.add_argument('--config', default='submit.cfg',
                        help=d)
    parser.add_argument('--full-log', default='full-ds-submit.log',
                        help=d)
    parser.add_argument('-i', '--increment-version', action='store_true')
    parser.add_argument(
        '-g', '--ngb', nargs='?', const='MAX',
        help='number of gb per job (with no arg: %(const)s )')
    parser.add_argument('--blacklist', nargs='*', default=[])
    parser.add_argument('--get-skimmer', action='store_true')
    parser.add_argument('--debug', action='store_true')
    return parser.parse_args()

def _get_datasets(ds_list_name):
    datasets = []
    if args.ds_list:
        with open(ds_list_name) as ds_list:
            for line in ds_list:
                dataset = line.split('#')[0].strip()
                if dataset:
                    datasets.append(dataset)

    elif not sys.stdin.isatty():
        for ds_name in sys.stdin.readlines():
            datasets.append(ds_name.strip())
    return datasets

# --- the main executable ---

if __name__ == '__main__':

    args = _get_args()
    config = _get_config(args.config)
    if not os.path.isfile(args.config):
        print 'writing config'
        with open(args.config,'w') as cfg:
            config.write(cfg)
        sys.exit('wrote config, exiting')

    version = config.get('version', 'version')
    tarball = 'jobtar.tar'
    branches = 'branches.txt'
    user = config.get('site','user') or os.path.expandvars('$USER')
    if args.get_skimmer:
        LocalSkimmer(branches).link()
        sys.exit('got skimmer, exiting...')

    if not find_executable('prun'):
        raise OSError("prun isn't defined, you need to set it up")

    if not os.path.isfile(branches):
        sys.exit('you need {}!'.format(branches))

    datasets = _get_datasets(args.ds_list)
    output_container_list = open(args.out_name,'w')
    out_log = open(args.full_log,'w')

    reporter = Reporter(len(datasets), out_log)
    reporter.start()
    def submit(ds):
        return submit_ds(ds, debug=args.debug, version=version,
                         branches=branches,
                         out_talk=reporter.queue,
                         in_tar=tarball,
                         maxgb=args.ngb,
                         blacklist=args.blacklist,
                         user=user)

    if not args.mono:
        pool = Pool(10)
        def map_func(sub, dss):
            return pool.map_async(sub, dss).get(999999)
    else:
        map_func = map
    with LocalSkimmer(branches=branches, tarball=tarball, user=user):
        out_tuples = map_func(submit, datasets)
    reporter.close()
    output_datasets = [ds for ds, out, err in out_tuples]
    for ds in output_datasets:
        output_container_list.write(ds + '\n')




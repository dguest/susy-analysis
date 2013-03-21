#!/usr/bin/env python2.7

"""
Script to submit prun jobs to skim D3PDs. 

Creates a skimmer.py in the current directory. 
"""

import sys, os, re
import argparse
from subprocess import Popen, PIPE
from multiprocessing import Pool, Process, Queue
from distutils.spawn import find_executable
from ConfigParser import SafeConfigParser

def _get_ptag(ds_name): 
    return re.compile('_p([0-9]{3,5})').findall(ds_name)[-1]

def _strip_front(ds_name): 
    if not any(ds_name.startswith(x) for x in ['data','mc']): 
        return _strip_front('.'.join(ds_name.split('.')[1:]))
    return ds_name

def submit_ds(ds_name, debug=False, version=0, used_vars='used_vars.txt', 
              out_talk=None, in_tar=None, maxgb=False): 

    user = os.path.expandvars('$USER')
    preskim_name = _strip_front(ds_name)
    output_base = '.'.join(preskim_name.split('.')[:3])
    rev_number = _get_ptag(ds_name)
    out_ds = 'user.{user}.{in_ds}.skim_p{rev}_v{version}/'.format(
        user=user, in_ds=output_base, version=version, rev=rev_number)

    run_string = 'python skimmer.py {} %IN'.format(used_vars)

    input_args = [
        '--inDS=' + ds_name,
        '--outDS=' + out_ds,
        '--outputs=skim-output.root,set-branches.txt', 
        '--excludeFile=*.tar,*.log,*.sh,*.out,*.root',
        '--extFile={}'.format(used_vars), 
        '--athenaTag=17.2.1', 
        ]
    if in_tar: 
        input_args.append('--inTarBal={}'.format(in_tar))
    if maxgb: 
        input_args.append('--nGBPerJob=MAX')

    exec_string = run_string

    
    submit_string = ['prun','--exec',exec_string] + input_args
    if debug:
        submit_string = ['echo'] + submit_string
    ps = Popen(submit_string, stdout=PIPE, stderr=PIPE)
    out, err = ps.communicate()
    if out_talk: 
        head_line = ' --- {} ---\n'.format(out_ds)
        out_talk.put(head_line + err + out + '\n')
    return out_ds, out, err

_skimmer = r"""
import sys
from ROOT import TChain, TFile, TTree

branches = open(sys.argv[1])

susy_chain = TChain('susy')
collection_chain = TChain('CollectionTree')

out_file = TFile('skim-output.root', 'recreate')

for f in sys.argv[2].split(','): 
    print 'adding', f
    ret = susy_chain.Add(f,-1)
    if ret != 1: 
        raise RuntimeError(f + ' susy is fucked')
    ret = collection_chain.Add(f,-1)
    if ret != 1: 
        raise RuntimeError(f + ' CollectionTree is fucked')

set_branches = open('set-branches.txt','w')

if not susy_chain.GetEntries(): 
    set_branches.write('none\n')
    print 'chain is empty, quitting...'
    sys.exit(0)

susy_chain.SetBranchStatus('*',0)
for branch in branches: 
    print 'setting', branch.strip()
    susy_chain.SetBranchStatus(branch.strip(), 1)
    set_branches.write(branch.strip() + '\n')
set_branches.close()

out_susy = susy_chain.CopyTree('{all_the_cuts}')
out_collection = collection_chain.CloneTree()

out_file.WriteTObject(out_susy)
out_file.WriteTObject(out_collection)
"""

class LocalSkimmer(object): 
    script_name = 'skimmer.py'
    def __init__(self, vars_file, all_the_cuts='', tarball=None): 
        self.all_the_cuts = all_the_cuts
        self.vars_file = vars_file
        self.tarball = tarball
    def __enter__(self): 
        if os.path.isfile(self.script_name): 
            os.remove(self.script_name)
        self.write()
        if self.tarball: 
            self.tar()
    def write(self): 
        with open(self.script_name,'w') as sk: 
            sk.write(_skimmer.format(all_the_cuts=self.all_the_cuts))
        return self
    def tar(self): 
        user = os.path.expandvars('$USER')
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
        os.remove(self.script_name)
        if self.tarball: 
            os.remove(self.tarball)
        
def get_config(config_name): 
    config = SafeConfigParser()
    config.read([config_name])
    if not config.has_section('version'): 
        config.add_section('version')
        config.set('version','version','0')
    if not config.has_section('variables'): 
        config.add_section('variables')
        config.set('variables','varlist','all-branches.txt')
    if not config.has_section('cuts'): 
        config.add_section('cuts')
        config.set('cuts', 'met_low', '70000')
    return config

def get_cuts_and_vars(cuts_dict): 
    cuts = []
    variables = []
    if 'met_low' in cuts_dict: 
        cuts.append('MET_RefFinal_et > {}'.format(cuts_dict['met_low']))
        variables.append('MET_RefFinal_et')
    return ' && '.join(cuts), variables

class Reporter(Process): 

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
        if self.output.isatty(): 
            self.output.write('\n')
    def close(self): 
        """
        to be called externally
        """
        self.queue.put('kill')
        self.queue.close()
        self.join()

if __name__ == '__main__': 
    parser = argparse.ArgumentParser(description=__doc__)

    d = 'default: %(default)s'
    parser.add_argument('ds_list', nargs='?')
    parser.add_argument('--mono', action='store_true',
                        help="don't multiprocess")
    parser.add_argument('--out-name', default='output-containers.txt', 
                        help=d)
    parser.add_argument('--config', default='submit.cfg', 
                        help=d)
    parser.add_argument('--full-log', default='full-ds-submit.log',
                        help=d)
    parser.add_argument('-i', '--increment-version', action='store_true')
    parser.add_argument('-g', '--ngb-max', action='store_true')
    parser.add_argument('--get-slimmer', action='store_true')
    parser.add_argument('--debug', action='store_true')
    args = parser.parse_args()

    config = get_config(args.config)
    if not os.path.isfile(args.config): 
        print 'writing config'
        with open(args.config,'w') as cfg: 
            config.write(cfg)
        sys.exit('wrote config, exiting')

    cuts, add_vars = get_cuts_and_vars(dict(config.items('cuts')))

    if args.get_slimmer: 
        LocalSkimmer(cuts).write()
        sys.exit('got skimmer, exiting...')

    if not find_executable('prun'): 
        raise OSError("prun isn't defined, you need to set it up")

    version = config.getint('version', 'version')
    if args.increment_version: 
        version += 1
        config.set('version','version',str(version))

    used_vars = config.get('variables','varlist')
    datasets = []

    if not os.path.isfile(used_vars): 
        sys.exit('you need {}!'.format(used_vars))

    if args.ds_list: 
        with open(args.ds_list) as ds_list: 
            for line in ds_list: 
                dataset = line.split('#')[0].strip()
                if dataset: 
                    datasets.append(dataset)

    elif not sys.stdin.isatty(): 
        for ds_name in sys.stdin.readlines(): 
            datasets.append(ds_name.strip())

    output_container_list = open(args.out_name,'w')
    out_log = open(args.full_log,'w')
    tarball = 'jobtar.tar'
    if not args.mono:
        reporter = Reporter(len(datasets), out_log)
        reporter.start()
        def submit(ds): 
            return submit_ds(ds, args.debug, version, 
                             used_vars=used_vars, 
                             out_talk=reporter.queue, 
                             in_tar=tarball, 
                             maxgb=args.ngb_max)

        pool = Pool(10)
        with LocalSkimmer(used_vars, cuts, tarball=tarball): 
            out_tuples = pool.map(submit, datasets)
        reporter.close()
        output_datasets = [ds for ds, out, err in out_tuples]
        for ds in output_datasets: 
            output_container_list.write(ds + '\n')
                          
    else: 
            
        for ds in datasets: 
            with LocalSkimmer(used_vars, cuts, tarball=tarball): 
                out_ds = submit_ds(ds, args.debug, version)
            output_container_list.write(out_ds + '\n')

    # record version if things worked
    with open(args.config,'w') as cfg: 
        config.write(cfg)




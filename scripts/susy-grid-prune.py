#!/usr/bin/env python2.7

"""
Script to submit prun jobs to skim D3PDs. 

Creates a skimmer.py in the current directory. 
"""

import sys, os, re, shutil
import argparse
from subprocess import Popen, PIPE
from multiprocessing import Pool, Process, Queue
from distutils.spawn import find_executable
from ConfigParser import SafeConfigParser

def _get_ptag(ds_name): 
    try: 
        return re.compile('_p([0-9]{3,5})').findall(ds_name)[-1]
    except IndexError: 
        return None

def _strip_front(ds_name): 
    if not any(ds_name.startswith(x) for x in ['data','mc']): 
        return _strip_front('.'.join(ds_name.split('.')[1:]))
    return ds_name

def submit_ds(ds_name, debug=False, version=0, used_vars='used_vars.txt', 
              out_talk=None, in_tar=None, maxgb=False, blacklist=[], 
              user=None): 

    if not user: 
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
        '--outputs=skim-output.root,set-branches.txt,n-entries.txt', 
        '--excludeFile=*.tar,*.log,*.sh,*.out,*.root',
        '--extFile={}'.format(used_vars), 
        '--rootVer=5.34/07', 
        '--cmtConfig=x86_64-slc5-gcc43-opt', 
        ]
    if blacklist: 
        input_args.append('--excludedSite={}'.format(','.join(blacklist)))
    if in_tar: 
        input_args.append('--inTarBal={}'.format(in_tar))
    if maxgb: 
        input_args.append('--nGBPerJob={}'.format(maxgb))

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
from ROOT import TChain, TFile, TTree, gDirectory, gROOT
gROOT.SetBatch()

branches = open(sys.argv[1])

susy_chain = TChain('{chain_name}')
collection_chain = TChain('CollectionTree')

out_file = TFile('skim-output.root', 'recreate')

for f in sys.argv[2].split(','): 
    print 'adding', f
    ret = susy_chain.Add(f,-1)
    ret = collection_chain.Add(f,-1)

set_branches = open('set-branches.txt','w')
entries = susy_chain.GetEntries()
susy_chain.Draw('0.5 >> sum_hist(1,0,1)','mcevt_weight[0][0]')
try: 
    wt_entries = gDirectory.Get('sum_hist').GetBinContent(1)
except AttributeError: 
    wt_entries = 0.0

entries_file = open('n-entries.txt','w')
entries_file.write(str(entries) + '\n')
entries_file.write(str(wt_entries) + '\n')
entries_file.close()

if not entries: 
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
    def __init__(self, vars_file, all_the_cuts='', tarball=None, 
                 chain='susy', user=None): 
        self.all_the_cuts = all_the_cuts
        self.vars_file = vars_file
        self.tarball = tarball
        self.chain = chain
        self.user = user
    def __enter__(self): 
        if os.path.isfile(self.script_name): 
            os.remove(self.script_name)
        self.write()
        if self.tarball: 
            self.tar()
        self.current_files = set(os.listdir('.'))
    def write(self): 
        with open(self.script_name,'w') as sk: 
            sk.write(_skimmer.format(
                    all_the_cuts=self.all_the_cuts,
                    chain_name=self.chain, 
                    branches_file=self.vars_file))
        return self
    def tar(self): 
        if self.user: 
            user = self.user
        else: 
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
    
        # clean up junk
        new_files = set(os.listdir('.')) - self.current_files
        for f in new_files: 
            if os.path.isdir(f): 
                shutil.rmtree(f)
            else: 
                os.remove(f)
        
def get_config(config_name, varlist_name): 
    if not varlist_name: 
        varlist_name = 'branches.txt'
    config = SafeConfigParser()
    config.read([config_name])

    if not config.has_section('version'): 
        config.add_section('version')
        config.set('version','version','0')
    if not config.has_section('variables'): 
        config.add_section('variables')
        config.set('variables','varlist', varlist_name)
        config.set('variables','chain','susy')
    if not config.has_section('cuts'): 
        config.add_section('cuts')
        config.set('cuts', 'met_low', '70000')
        config.set('cuts','mark_skim', 'true')
    if not config.has_section('local'): 
        config.add_section('local')
        config.set('local','user',os.path.expandvars('$USER'))
    return config

def get_cuts_and_vars(cuts_dict): 
    if cuts_dict.get('mark_skim','false') == 'true': 
        return get_marks_skim(cuts_dict)
    cuts = {}
    variables = []
    if 'met_low' in cuts_dict: 
        cuts['met_low'] = 'MET_RefFinal_et > {}'.format(
            cuts_dict['met_low'])
        variables.append('MET_RefFinal_et')
    if 'triggers' in cuts_dict: 
        triggers = cuts_dict['triggers'].split()
        for trig in triggers: 
            variables.append(trig)
        cuts['triggers'] = '( {} )'.format(' || '.join(triggers))
    if 'trigger' in cuts_dict: 
        trigger = cuts_dict['trigger']
        variables.append(trigger)
        cuts['trigger'] = trigger
    if 'n_staco' in cuts_dict: 
        cuts['n_staco'] = 'mu_staco_n >= {}'.format(cuts_dict['n_staco'])

    ored_cuts = set(cuts_dict.get('or','').split())
    all_cuts = set(cuts.keys())
    anded_cuts = all_cuts - ored_cuts - set(['or'])

    or_cuts = [cuts[x] for x in ored_cuts]
    and_cuts = [cuts[x] for x in anded_cuts]
    if or_cuts: 
        or_str = ' || '.join(or_cuts)
        and_cuts.append('( {} )'.format(or_str))
    return ' && '.join(and_cuts), variables

def get_marks_skim(cuts_dict): 
    met_final = 'MET_Egamma10NoTau_RefFinal'
    met_muon = 'MET_Egamma10NoTau_Muon_Total_Staco'
    triggers = [
        'EF_xe80_tclcw_tight',  # main trigger (should work all year)
        'EF_mu24i_tight',          # for Z CR
        ]
    met_final_exp = '{f}_etx * {f}_etx + {f}_ety * {f}_ety'.format(
        f=met_final)
    met_muon_exp = (
        '({f}_etx - {m}_etx)*({f}_etx - {m}_etx) + ' 
        '({f}_ety - {m}_ety)*({f}_ety - {m}_ety)' 
        ).format(f=met_final, m=met_muon)
    met_final_str = '(sqrt({}) > {})'.format(
        met_final_exp, cuts_dict['met_low'])
    met_muon_str = '(sqrt({}) > {})'.format(
        met_muon_exp, cuts_dict['met_low'])
    trig_or = ' || '.join(triggers)
    met_or = ' {} || {} '.format(met_final_str, met_muon_str)
    branches = triggers + [
        '{}_et{}'.format(m,c) for m in [met_final, met_muon] for c in 'xy']
    final = '({}) && ({})'.format(trig_or, met_or)

    return final, branches

def add_vars(new_vars, file_name): 
    """
    adds new_vars to end of file 'file_name' if they aren't there already
    """
    with open(file_name, 'r+') as f: 
        all_vars = set(x.strip() for x in f.readlines())
        missing_vars = set(new_vars) - all_vars
        for var in missing_vars: 
            f.write(var + '\n')

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
    parser.add_argument('--get-slimmer', action='store_true')
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

if __name__ == '__main__': 

    args = _get_args()
    config = get_config(args.config, args.variable_list)
    if not os.path.isfile(args.config): 
        print 'writing config'
        with open(args.config,'w') as cfg: 
            config.write(cfg)
        sys.exit('wrote config, exiting')

    cuts, new_variables = get_cuts_and_vars(dict(config.items('cuts')))

    chain_name = config.get('variables','chain')
    if args.get_slimmer: 
        LocalSkimmer(args.ds_list, all_the_cuts=cuts, 
                     chain=chain_name).write()
        sys.exit('got skimmer, exiting...')

    if not find_executable('prun'): 
        raise OSError("prun isn't defined, you need to set it up")

    version = config.getint('version', 'version')
    if args.increment_version: 
        version += 1
        config.set('version','version',str(version))
    user = config.get('local','user')

    used_vars = args.variable_list
    if not used_vars: 
        used_vars = config.get('variables','varlist')
    add_vars(new_variables, used_vars)

    if not os.path.isfile(used_vars): 
        sys.exit('you need {}!'.format(used_vars))

    datasets = _get_datasets(args.ds_list)

    output_container_list = open(args.out_name,'w')
    out_log = open(args.full_log,'w')
    tarball = 'jobtar.tar'

    reporter = Reporter(len(datasets), out_log)
    reporter.start()
    def submit(ds): 
        return submit_ds(ds, args.debug, version, 
                         used_vars=used_vars, 
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
    with LocalSkimmer(used_vars,cuts,tarball=tarball,chain=chain_name, 
                      user=user): 
        out_tuples = map_func(submit, datasets)
    reporter.close()
    output_datasets = [ds for ds, out, err in out_tuples]
    for ds in output_datasets: 
        output_container_list.write(ds + '\n')
                          

    # record version if things worked
    with open(args.config,'w') as cfg: 
        config.write(cfg)




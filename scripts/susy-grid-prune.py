#!/usr/bin/env python2.7

"""
Script to submit prun jobs to skim D3PDs. 

Uses filter-and-merge-d3pd.py
"""

import sys, os, re
import argparse
from subprocess import Popen, PIPE
from multiprocessing import Pool
from distutils.spawn import find_executable
from ConfigParser import SafeConfigParser

def _get_ptag(ds_name): 
    return re.compile('_p([0-9]{3,5})').search(ds_name).group(1)

def submit_ds(ds_name, debug=False, version=3, used_vars='used_vars.txt'): 

    user = os.path.expandvars('$USER')
    if ds_name.startswith('user'): 
        preskim_name = '.'.join(ds_name.split('.')[2:])
    else: 
        preskim_name = ds_name
    output_base = '.'.join(preskim_name.split('.')[:3])
    rev_number = _get_ptag(ds_name)
    out_ds = 'user.{user}.{in_ds}.skim_p{rev}_v{version}/'.format(
        user=user, in_ds=output_base, version=version, rev=rev_number)

    build_string = 'echo %IN | sed \'s/,/\\n/g\' > grid_files.txt'

    run_args = [
        '--in=grid_files.txt',
        '--out=skim-output.root', 
        '--tree=susy', 
        '--keep-all-trees', 
        '--var={}'.format(used_vars), 
        ]
    run_string = 'filter-and-merge-d3pd.py ' + ' '.join(run_args)

    input_args = [
        '--inDS=' + ds_name,
        '--outDS=' + out_ds,
        '--outputs=skim-output.root', 
        '--excludeFile=*.tar,*.log,*.sh,*.py,*.out,*.root',
        '--extFile={}'.format(used_vars), 
        '--athenaTag=17.2.1', 
        ]

    exec_string = '' + '; '.join([build_string, run_string]) + ''

    
    submit_string = ['prun','--exec',exec_string] + input_args
    if debug:
        submit_string = ['echo'] + submit_string
    ps = Popen(submit_string)
    ps.communicate()
    return out_ds

if __name__ == '__main__': 
    parser = argparse.ArgumentParser(description=__doc__)

    parser.add_argument('ds_list', nargs='?')
    parser.add_argument('--debug', action='store_true')
    parser.add_argument('--mono', action='store_true',
                        help="don't multiprocess")
    parser.add_argument('--out-name', default='output_datasets.txt', 
                        help='default: %(default)s')
    parser.add_argument('--varlist', default='used_vars.txt', 
                        help='list of variables to include '
                        'default: %(default)s')
    parser.add_argument('--config', default='submit.cfg', 
                        help='default: %(default)s')
    args = parser.parse_args()

    config = SafeConfigParser()
    config.read([args.config])
    if not config.has_section('version'): 
        config.add_section('version')
        config.set('version','version','0')
        with open(args.config,'w') as cfg: 
            config.write(cfg)
    if not config.has_section('variables'): 
        config.add_section('variables')
        config.set('variables','varlist',args.varlist)

    version = config.getint('version', 'version')
    config.set('version','version',str(version + 1))

    used_vars = config.get('variables','varlist')

    if not find_executable('prun'): 
        raise OSError("prun isn't defined, you need to set it up")
    
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

    output_datasets_list = open(args.out_name,'w')

    if not args.mono:
        def submit(ds): 
            return submit_ds(ds, args.debug, version, used_vars=used_vars)

        pool = Pool(10)
        output_datasets = pool.map(submit, datasets)
        for ds in output_datasets: 
            output_datasets_list.write(ds + '\n')
    else: 
            
        for ds in datasets: 
            out_ds = submit_ds(ds, args.debug, version)
            output_datasets_list.write(out_ds + '\n')

    # record version if things worked
    with open(args.config,'w') as cfg: 
        config.write(cfg)

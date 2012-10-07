#!/usr/bin/env python2.7

"""
Script to submit prun jobs to skim D3PDs
"""

import sys, os
import argparse
from subprocess import Popen, PIPE
from multiprocessing import Pool

def submit_ds(ds_name, debug=False, version=3, used_vars='used_vars.txt'): 

    user = os.path.expandvars('$USER')
    output_base = '.'.join(ds_name.split('.')[:4])
    out_ds = 'user.{user}.{in_ds}.skim_v{version}/'.format(
        user=user, in_ds=output_base, version=version)

    build_string = 'echo %IN | sed \'s/,/\\n/g\' > grid_files.txt'

    run_args = [
        '--in=grid_files.txt',
        '--out=skim-output.root', 
        '--tree=susy', 
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
    parser.add_argument('-v', default='0', help='version')
    parser.add_argument('--debug', action='store_true')
    parser.add_argument('--multi', action='store_true')
    parser.add_argument('--out_name', default='output_datasets.txt', 
                        help='default: %(default)s')
    parser.add_argument('--varlist', default='used_vars.txt', 
                        help='list of variables to include '
                        'default: %(default)s')
    args = parser.parse_args()
    
    datasets = []

    if not os.path.isfile(args.varlist): 
        sys.exit('you need {}!'.format(args.varlist))

    if args.ds_list: 
        with open(args.ds_list) as ds_list: 
            for line in ds_list: 
                dataset = line.split('#')[0].strip()
                if dataset: 
                    datasets.append(dataset)

    if len(sys.argv) == 1: 
        for ds_name in sys.stdin.readlines(): 
            datasets.append(ds_name.strip())

    output_datasets_list = open(args.out_name,'w')

    if args.multi:
        def submit(ds): 
            return submit_ds(ds, args.debug, args.v, used_vars=args.varlist)

        pool = Pool(10)
        output_datasets = pool.map(submit, datasets)
        for ds in output_datasets: 
            output_datasets_list.write(ds + '\n')
    else: 
            
        for ds in datasets: 
            out_ds = submit_ds(ds, args.debug, args.v)
            output_datasets_list.write(out_ds + '\n')

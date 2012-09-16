#!/usr/bin/env python2.7

import sys, os
import argparse
from subprocess import Popen, PIPE

def submit_ds(ds_name, debug=False): 

    user = os.path.expandvars('$USER')
    output_base = ds_name.strip('/')
    out_ds = 'user.{user}.{in_ds}.skim'.format(user=user,in_ds=output_base)

    build_string = 'echo %IN | sed \'s/,/\n/g\' > grid_files.txt'

    run_args = [
        '--in=grid_files.txt',
        '--out=skim-output.root', 
        '--tree=susy', 
        '--var=used_vars.txt', 
        ]
    run_string = 'filter-and-merge-d3pd.py ' + ' '.join(run_args)

    input_args = ['--inDS=' + ds_name,
                  '--outDS=' + out_ds,
                  '--outputs="skim-output_*.root"', 
                  '--excludeFile=*.tar,*.log,*.sh,*.py,*.out',
                  '--athenaTag=17.2.1']

    exec_string = '\"' + '; '.join([build_string, run_string]) + '\"'

    
    submit_string = ['prun','--exec',exec_string] + input_args
    if debug:
        submit_string = ['echo'] + submit_string
    ps = Popen(submit_string)
    ps.communicate()

if __name__ == '__main__': 
    parser = argparse.ArgumentParser()

    parser.add_argument('ds_list', nargs='?')
    parser.add_argument('--debug', action='store_true')
    args = parser.parse_args()
    
    datasets = []

    if args.ds_list: 
        with open(args.ds_list) as ds_list: 
            for line in ds_list: 
                datasets.append(line.strip())

    if len(sys.argv) == 1: 
        for ds_name in sys.stdin.readlines(): 
            datasets.append(ds_name.strip())

    for ds in datasets: 
        submit_ds(ds, args.debug)

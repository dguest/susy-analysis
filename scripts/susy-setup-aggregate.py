#!/usr/bin/env python3.3
"""Setup the aggregator"""

import argparse, sys
import os
import glob
from os.path import isdir, join, dirname
from scharm.schema import get_all_systematics
from shutil import rmtree

def get_config():
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        'hists_dir',
        help='top level post-hadd directory')
    parser.add_argument('-m', '--meta', required=True)
    parser.add_argument(
        '-s', '--script', help='build this PBS script ' + d,
        default='megashark.sh')
    parser.add_argument('-o', '--out-dir', default='aggregate', help=d)
    return parser.parse_args(sys.argv[1:])

def run():
    config = get_config()
    setup_aggregate(config)

# __________________________________________________________________________
# stuff from the setup part

def setup_aggregate(config):
    """
    Build the distiller submit script.
    """
    systs = get_all_systematics(config.hists_dir)
    sub_dict = {
        'n_jobs': len(systs),
        'out_dir': 'output/aggregate',
        'walltime': '00:03:00:00',
        'odir': 'config.out_dir',
        }

    submit_head = _get_submit_head(**sub_dict)
    if isdir(config.out_dir):
        rmtree(config.out_dir)
    os.mkdir(config.out_dir)

    with open(config.script, 'w') as out_script:
        out_script.write(submit_head)
        out_script.write('systs=(\n')
        for syst in systs:
            out_script.write(syst + '\n')
        out_script.write(')\n')
        sub_line = 'susy-aggregate.py -m {meta} -o {odir}/{syst}.h5 -s {syst}'
        sub_line += ' {} -q'.format(config.hists_dir)
        out_script.write(sub_line.format(
                meta=config.meta, odir=config.out_dir,
                syst='${systs[$PBS_ARRAYID-1]}'))

# TODO: Should move this into a common place

_short_subhead="""
#!/usr/bin/env bash

#PBS -t 1-{n_jobs}
#PBS -l mem={mem},walltime={walltime}
#PBS -l nodes=1:ppn=1
#PBS -q hep
#PBS -o {out_dir}/out.txt
#PBS -e {out_dir}/error.txt

cd $PBS_O_WORKDIR
mkdir -p {out_dir} {odir}
echo 'submitted from: ' $PBS_O_WORKDIR

"""


def _get_submit_head(n_jobs, out_dir, **args):
    default_args = dict(mem='4gb', walltime='00:06:00:00')
    default_args.update(args)
    subhead = _short_subhead
    return subhead.format(n_jobs=n_jobs, out_dir=out_dir, **default_args)

if __name__ == '__main__':
    run()

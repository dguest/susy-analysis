#!/usr/bin/env python3.3
"""Setup the distiller"""

_group_help="""
Groups d3pds into datasets by file. Will break larger datasets into multiple
outputs.
"""

import argparse, sys
import os
import random
import glob
from collections import defaultdict
import re
from os.path import isdir, join, dirname
from shutil import rmtree

from scharm.schema import get_jes_variations

def get_config():
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        'input_dirs', nargs='+',
        help='dataset directories, must each have a dsid')
    parser.add_argument(
        '-g', '--gb-per-job', type=float, default=2.0,
        help='number of root files to put in each out file, ' + d)
    parser.add_argument('-o', '--batch-dir', default='batch/d3pds',
                       help=d)
    parser.add_argument('-m', '--meta', required=True)
    parser.add_argument(
        '-s', '--script', help='build this PBS script ' + d,
        default='sharktopus.sh')
    parser.add_argument('-p', '--build-prw', action='store_true')
    parser.add_argument('-a', '--aggressive', action='store_true')
    syslev = parser.add_mutually_exclusive_group()
    syslev.add_argument('-y', '--systematics', action='store_true',
                        help='enable systematics')
    syslev.add_argument('--all-systematics', action='store_true')
    syslev.add_argument('--jes-systematics', action='store_true')
    return parser.parse_args(sys.argv[1:])

def run():
    config = get_config()
    if isdir(config.batch_dir):
        rmtree(config.batch_dir)
    input_files = group_input_files(config)
    setup_distill(config, input_files)

# __________________________________________________________________________
# stuff from the utils part

def _file_group_gen(files, chunk_size):
    """chunks files into subsets based on size"""
    tot_size = 0.0
    out_ntuples = []
    for path in files:
        size = os.path.getsize(path)
        if tot_size + size > chunk_size and out_ntuples:
            yield out_ntuples
            tot_size = 0.0
            out_ntuples = []
        out_ntuples.append(path)
        tot_size += size
    if out_ntuples:
        yield out_ntuples

def group_input_files(config):
    from scharm.schema import get_prechar
    files_by_dsid = defaultdict(set)
    dsid_finder = re.compile('mc.._.TeV\.([0-9]{6})\.')
    run_finder = re.compile('data.._.TeV\.([0-9]{8})\.')
    for in_dir in config.input_dirs:
        prechar = get_prechar(in_dir)
        dsid_match = dsid_finder.search(in_dir)
        if not dsid_match:
            dsid_match = run_finder.search(in_dir)
        if not dsid_match:
            raise IOError("can't find dsid for {}".format(in_dir))
        ds_key = prechar + dsid_match.group(1).lstrip('0')
        sub_root_files = set(glob.glob('{}/*.root*'.format(in_dir)))
        files_by_dsid[ds_key] |= sub_root_files

    if not isdir(config.batch_dir):
        os.makedirs(config.batch_dir)

    out_files = []
    for dsid, files in files_by_dsid.items():
        file_list = sorted(files)
        random.shuffle(file_list)
        subsets = []
        chunk_size = config.gb_per_job * 1e9
        for file_group in _file_group_gen(file_list, chunk_size):
            subsets.append(file_group)
        n_subsets = len(subsets)
        strlen_subsets = len(str(n_subsets))
        for set_n, subset in enumerate(subsets,1):
            out_name = '{}-{:0{}d}of{}.txt'.format(
                dsid, set_n, strlen_subsets, n_subsets)
            if n_subsets == 1:
                out_name = '{}.txt'.format(dsid)
            out_path = join(config.batch_dir, out_name)
            with open(out_path,'w') as out_file:
                for ds in subset:
                    out_file.write(ds + '\n')
            out_files.append(out_path)
    return out_files

# __________________________________________________________________________
# stuff from the setup part

def setup_distill(config, input_files):
    """
    Build the distiller submit script.
    """
    in_dir = dirname(input_files[0])
    sub_dict = {
        'n_jobs': len(input_files),
        'out_dir': 'output/distill',
        'in_dir': in_dir,
        'in_ext': '.txt',
        'walltime': '00:08:00:00'
        }

    systematics = ['NONE']
    ud_systs = []
    base_syst_trig = [
        config.systematics, config.all_systematics, config.jes_systematics]
    if any(base_syst_trig):
        systematics += ['JER','METRES']
        ud_systs += ['JES', 'MET','EGZEE', 'EGLOW', 'MSCALE', 'JVF']
    if config.all_systematics:
        ud_systs += ['EGMAT', 'EGPS', 'EGRES', 'MMS', 'MID']
    if config.all_systematics or config.jes_systematics:
        ud_systs += get_jes_variations()

    systematics += [s + e for s in ud_systs for e in ['UP','DOWN']]

    submit_head = _get_submit_head(**sub_dict)

    with open(config.script, 'w') as out_script:
        out_script.write(submit_head)
        for syst in systematics:
            run_args = [
                config.meta,
                '-s {}'.format(syst),
                '-o ntuples']
            line_args = {
                'routine': 'susy-distill.py',
                'run_args': ' '.join(run_args),
                }
            syst_line = _submit_line.format(**line_args)
            out_script.write(syst_line + '\n')

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
mkdir -p {out_dir}
echo 'submitted from: ' $PBS_O_WORKDIR

"""

# do fullsim first
_sortstr = "sed 's/^s/0/' | sort | sed 's/^0/s/' | sort"
_file_getter = "files=($(ls {in_dir}/*{in_ext} | " + _sortstr + "))\n"
_submit_head = _short_subhead + _file_getter

def _get_submit_head(n_jobs, out_dir, short=False, **args):
    default_args = dict(
        in_dir='.', in_ext='.txt', mem='4gb', walltime='00:06:00:00')
    default_args.update(args)
    subhead = _short_subhead if short else _submit_head
    return subhead.format(n_jobs=n_jobs, out_dir=out_dir, **default_args)




_submit_line = '{routine} ${{files[$PBS_ARRAYID-1]}} {run_args}'



if __name__ == '__main__':
    run()

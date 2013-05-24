#!/usr/bin/env python2.7
"""
Omega setup scripts
"""
import argparse, sys
from stop import meta

def run(): 
    config = get_config()
    subs = {'distill': setup_distill}
    subs[config.which](config)

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')
    distil_args = subs.add_parser('distill')
    distil_args.add_argument(
        'input_textfiles', nargs='+', 
        help='can be produced with susy-utils')
    distil_args.add_argument('-m', '--update-meta', required=True, 
                           help='update meta file with sum_wt etc')
    distil_args.add_argument('-s', '--script', help='build this PBS script')
    return parser.parse_args(sys.argv[1:])

def setup_distill(config): 
    ds_meta = None
    if config.update_meta: 
        ds_meta = meta.DatasetCache(config.update_meta)
        for ds_key in ds_meta: 
            ds_meta[ds_key].sum_event_weight = 0.0
            ds_meta[ds_key].n_raw_entries = 0
        collector = meta.MetaTextCollector()
        out_meta = meta.DatasetCache()
        for textfile in config.input_textfiles: 
            ds_key = basename(splitext(textfile)[0]).split('-')[0]
            with open(textfile) as steering_file: 
                files = [l.strip() for l in steering_file.readlines()]
            n_events, total_wt, n_cor = collector.get_recorded_events(files)
            out_meta[ds_key] = ds_meta[ds_key]
            out_meta[ds_key].sum_event_weight += total_wt
            out_meta[ds_key].n_raw_entries += n_events
        out_meta.write(config.update_meta)
    if config.script: 
        run_args = config.update_meta
        sub_dict = {
            'n_jobs': len(config.input_textfiles), 
            'out_dir': 'distill-output', 
            'in_dir': 'd3pds', 
            'in_ext': '.txt', 
            'routine': 'susy-distill.py', 
            'run_args': run_args, 
            }
        submit_script = _submit_script.format(**sub_dict)
        with open(config.script, 'w') as out_script: 
            out_script.write(submit_script)

_submit_script="""
#!/usr/bin/env bash

#PBS -t 1-{n_jobs}
#PBS -l mem=4gb,walltime=00:06:00:00
#PBS -l nodes=1:ppn=1
#PBS -q hep
#PBS -o {out_dir}/out.txt
#PBS -e {out_dir}/error.txt

cd $PBS_O_WORKDIR
mkdir -p {out_dir}
echo 'submitted from: ' $PBS_O_WORKDIR 

files=($(ls {in_dir}/*{in_ext} | sort))

{routine} ${{files[$PBS_ARRAYID-1]}} {run_args}
"""

if __name__ == '__main__': 
    run()


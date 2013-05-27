#!/usr/bin/env python2.7
"""
Omega setup scripts
"""
import argparse, sys, os
from stop import meta
from os.path import basename, splitext, dirname, isdir

def run(): 
    config = get_config()
    subs = {'distill': setup_distill, 'stack': setup_stack, 
            'hadd': setup_hadd}
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

    stack_args = subs.add_parser('stack')
    stack_args.add_argument('input_ntuples', nargs='+')
    stack_args.add_argument('-y', '--steering', required=True)
    stack_args.add_argument('-s', '--script', help='build this', 
                            default='crocosaurus.sh')
    stack_args.add_argument('-o', '--output-name', 
                            default='ntuple-list/ntuples.txt', help=d)
    stack_args.add_argument('-n','--n-outputs', type=int, default=20, help=d)

    stack_args = subs.add_parser('hadd')
    stack_args.add_argument('input_dirs', nargs='+')
    stack_args.add_argument('-s', '--script', help='build this', 
                            default='varzo.sh')

    return parser.parse_args(sys.argv[1:])

def setup_hadd(config): 
    sys.exit('not implemented')

def setup_stack(config): 
    all_files = config.input_ntuples
    subfiles = {x:[] for x in xrange(config.n_outputs)}
    for in_n, in_file in enumerate(all_files): 
        subfiles[in_n % config.n_outputs].append(in_file)
    for file_n in xrange(config.n_outputs): 
        file_name = '{}-{n}{}'.format(
            *splitext(config.output_name), n=file_n)
        if not isdir(dirname(file_name)): 
            os.mkdir(dirname(file_name))
        with open(file_name, 'w') as out_file: 
            out_file.writelines('\n'.join(subfiles[file_n]) + '\n')

    sub_dict = {
        'n_jobs': config.n_outputs, 
        'out_dir': 'stack-output', 
        'in_dir': dirname(config.output_name), 
        'in_ext': '.txt', 
        }
    submit_head = _submit_head.format(**sub_dict)

    line_args = { 
        'routine': 'susy-stack.py run', 
        'run_args': config.steering
        }
    run_line = _submit_line.format(**line_args)

    with open(config.script, 'w') as out_script: 
        out_script.write(submit_head)
        out_script.write(run_line + '\n')

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
        _write_distill_config(script_name=config.script, 
                              meta_name=config.update_meta, 
                              input_files=config.input_textfiles)


def _dirify(systematic_name): 
    if systematic_name == 'NONE': 
        return 'baseline'
    return systematic_name.lower()

def _write_distill_config(script_name, meta_name, input_files, 
                          systematic='all'): 
    sub_dict = {
        'n_jobs': len(input_files), 
        'out_dir': 'distill-output', 
        'in_dir': 'd3pds', 
        'in_ext': '.txt', 
        }

    if systematic == 'all': 
        systematics = ['JESUP', 'JESDOWN', 'JER', 'NONE']
    else: 
        systematics = [systematic]

    submit_head = _submit_head.format(**sub_dict)

    with open(script_name, 'w') as out_script: 
        out_script.write(submit_head)
        for syst in systematics:
            run_args = [
                meta_name, 
                '-s {}'.format(syst), 
                '-o whiskey/{}'.format(_dirify(syst))]
            line_args = { 
                'routine': 'susy-distill.py', 
                'run_args': ' '.join(run_args), 
                }
            syst_line = _submit_line.format(**line_args)
            out_script.write(syst_line + '\n')
    

_submit_head="""
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

"""
_submit_line = '{routine} ${{files[$PBS_ARRAYID-1]}} {run_args}'

if __name__ == '__main__': 
    run()


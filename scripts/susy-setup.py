#!/usr/bin/env python2.7
"""
Omega setup scripts
"""
import argparse, sys, os
from stop import meta, bullshit
from os.path import basename, splitext, dirname, isdir, isfile

def run(): 
    config = get_config()
    subs = {'distill': setup_distill, 'stack': setup_stack, 
            'hadd': setup_hadd, 'fit': setup_fit, 'ws': setup_workspaces}
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
    distil_args.add_argument('-a', '--aggressive', action='store_true')

    stack_args = subs.add_parser('stack', description=setup_stack.__doc__)
    stack_args.add_argument('input_ntuples', nargs='+')
    stack_args.add_argument('-y', '--steering', required=True)
    stack_args.add_argument('-s', '--script', help='build this, ' + d, 
                            default='crocosaurus.sh')
    stack_args.add_argument('-o', '--output-name', 
                            default='batch/ntuple/ntuples.txt', help=d)
    stack_args.add_argument('-n','--n-outputs', type=int, default=20, help=d)

    stack_args = subs.add_parser('hadd', description=setup_hadd.__doc__)
    stack_args.add_argument('input_dirs', nargs='+')
    stack_args.add_argument('-o', '--textfile-name', 
                            default='batch/hadddir/hadd.txt', help=d)
    stack_args.add_argument('-s', '--script', help='build this, ' + d, 
                            default='varzo.sh')
    stack_args.add_argument('-d', '--hadd-dir', default='hadd-hists', 
                            help=d)

    ws_args = subs.add_parser('ws', description=setup_workspaces.__doc__)
    ws_args.add_argument('kinematic_stat_dir')

    fit_args = subs.add_parser('fit', description=setup_fit.__doc__)
    fit_args.add_argument('root')
    fit_args.add_argument('-t','--text-file',action='store_true', 
                          help='make steering text file')
    fit_args.add_argument('-s','--sql-db', action='store_true')

    return parser.parse_args(sys.argv[1:])

def setup_workspaces(config): 
    """
    Setup jobs to build workspaces for fitting. 
    """
    from pyroot.fitter import CountDict
    counts = CountDict(config.kinematic_stat_dir, systematics=[])
    n_sp = len({k[1] for k in counts if 'stop' in k[1]})

    sub_dict = {
        'n_jobs': n_sp, 
        'out_dir': 'output/workspace', 
        'in_dir': '.', 
        'in_ext': '.txt', 
        'walltime': '30:00', 
        }
    submit_head = _get_submit_head(**sub_dict)
    submit_line = 'susy-histopt.py ms {} -s $(($PBS_ARRAYID-1))'.format(
        config.kinematic_stat_dir)
    with open('leaky-rootf.sh','w') as the_job: 
        the_job.write(submit_head + '\n')
        the_job.write(submit_line + '\n')

def _build_fit_batch(root_dir, batch_path): 
    n_jobs = 0
    model_name = 'stop_combined_meas_model.root'
    prog_meter = bullshit.ProgressMeter()
    with open(batch_path, 'w') as bfile: 
        for root, subdirs, files in bullshit.fast_walk(root_dir): 
            if not model_name in files: 
                continue
            prog_meter.get_walk_progress(root)
            n_jobs += 1
            full_path = os.path.join(root, model_name)
            bfile.write(full_path + '\n')
    return n_jobs

def setup_fit(config): 
    """
    Sets up histfitter jobs. Will run one job for each leaf directory given 
    under the root directory. 
    """
    batch_dir = 'batch/fits'
    batch_file = 'fit-paths.txt'

    if not isdir(batch_dir): 
        os.makedirs(batch_dir)
    
    batch_path = os.path.join(batch_dir, batch_file)
    if config.text_file: 
        n_jobs = _build_fit_batch(config.root, batch_path)
    elif not isfile(batch_path): 
        raise OSError("no batch file found at {}".format(batch_path))
    else: 
        n_jobs = 0
        with open(batch_path) as bfile: 
            for line in bfile: 
                if line.strip(): 
                    n_jobs += 1
    sub_dict = {
        'n_jobs': n_jobs, 
        'out_dir': 'output/fit', 
        'in_dir': batch_dir, 
        'walltime': '30:00', 
        }
    submit_head = _get_submit_head(**sub_dict)

    line_args = { 
        'routine': 'susy-histopt.py ul', 
        'run_args': '--save-yaml' 
        }
    submit_line = 'susy-histopt.py ul {} -n $(($PBS_ARRAYID-1))'.format(
        batch_path)
    with open('all-the-fits.sh','w') as fits: 
        fits.write(submit_head + '\n')
        fits.write(submit_line + '\n')
    if config.sql_db: 
        sql_name = 'all-fit-results.db'
        from pyroot.fitter import make_sql
        if isfile(sql_name): 
            os.remove(sql_name)
        make_sql(sql_name)

def setup_hadd(config): 
    """
    Sets up text files and shell script to run hadding via susy-utils. 
    Creates one job for each directory to hadd. 
    """
    for dir_n, hadd_dir in enumerate(config.input_dirs): 
        if not isdir(hadd_dir): 
            raise OSError("inputs must be directories")
        txt_file_name = '{}-{n}{}'.format(*splitext(config.textfile_name), 
                                           n=dir_n)
        if not isdir(dirname(txt_file_name)): 
            os.makedirs(dirname(txt_file_name))
        with open(txt_file_name, 'w') as out_file: 
            out_file.write(hadd_dir + '\n')

    sub_dict = {
        'n_jobs': len(config.input_dirs), 
        'out_dir': 'output/hadd', 
        'in_dir': dirname(config.textfile_name), 
        'in_ext': '.txt', 
        }
    submit_head = _get_submit_head(**sub_dict)
    line_args = { 
        'routine': 'susy-util.py hadd', 
        'run_args': '--recursive --output {}'.format(config.hadd_dir), 
        }
    submit_line = '{routine} $(cat ${{files[$PBS_ARRAYID-1]}}) {run_args}'
    run_line = submit_line.format(**line_args)

    with open(config.script, 'w') as out_script: 
        out_script.write(submit_head)
        out_script.write(run_line + '\n')

def setup_stack(config): 
    """
    Sets up textfiles and shell script to run histograming via susy-stack.
    One histogram file will be created for each input ntuple. 
    """
    all_files = config.input_ntuples
    subfiles = {x:[] for x in xrange(config.n_outputs)}
    for in_n, in_file in enumerate(all_files): 
        subfiles[in_n % config.n_outputs].append(in_file)
    for file_n in xrange(config.n_outputs): 
        file_name = '{}-{n}{}'.format(
            *splitext(config.output_name), n=file_n)
        if not isdir(dirname(file_name)): 
            os.makedirs(dirname(file_name))
        with open(file_name, 'w') as out_file: 
            out_file.writelines('\n'.join(subfiles[file_n]) + '\n')

    sub_dict = {
        'n_jobs': config.n_outputs, 
        'out_dir': 'output/stack', 
        'in_dir': dirname(config.output_name), 
        'in_ext': '.txt', 
        }
    submit_head = _get_submit_head(**sub_dict)

    def get_runline(mode): 
        line_args = { 
            'routine': 'susy-stack.py run', 
            'run_args': '{} --mode {}'.format(config.steering, mode), 
            }
        return _submit_line.format(**line_args) + '\n'

    with open(config.script, 'w') as out_script: 
        out_script.write(submit_head)
        out_script.write(get_runline('histmill'))
        out_script.write(get_runline('kinematic_stat'))

def setup_distill(config): 
    ds_meta = None
    if config.update_meta: 
        ds_meta = meta.DatasetCache(config.update_meta)
        for ds_key in ds_meta: 
            ds_meta[ds_key].sum_event_weight = 0.0
            ds_meta[ds_key].n_raw_entries = 0
        collector = meta.MetaTextCollector()
        for textfile in config.input_textfiles: 
            ds_key = basename(splitext(textfile)[0]).split('-')[0]
            with open(textfile) as steering_file: 
                files = [l.strip() for l in steering_file.readlines()]
            n_events, total_wt, n_cor = collector.get_recorded_events(
                files, aggressive=config.aggressive)
            ds_meta[ds_key].sum_event_weight += total_wt
            ds_meta[ds_key].n_raw_entries += n_events
        ds_meta.write(config.update_meta)
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
    in_dir = dirname(input_files[0])
    sub_dict = {
        'n_jobs': len(input_files), 
        'out_dir': 'output/distill', 
        'in_dir': in_dir, 
        'in_ext': '.txt', 
        }

    if systematic == 'all': 
        systematics = ['JESUP', 'JESDOWN', 'JER', 'NONE']
    else: 
        systematics = [systematic]

    submit_head = _get_submit_head(**sub_dict)

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
#PBS -l mem={mem},walltime={walltime}
#PBS -l nodes=1:ppn=1
#PBS -q hep
#PBS -o {out_dir}/out.txt
#PBS -e {out_dir}/error.txt

cd $PBS_O_WORKDIR
mkdir -p {out_dir}
echo 'submitted from: ' $PBS_O_WORKDIR 

files=($(ls {in_dir}/*{in_ext} | sort))

"""

def _get_submit_head(n_jobs, out_dir, **args): 
    default_args = dict(
        in_dir='.', in_ext='.txt', mem='4gb', walltime='00:06:00:00')
    default_args.update(args)
    return _submit_head.format(n_jobs=n_jobs, out_dir=out_dir, **default_args)

        
    

_submit_line = '{routine} ${{files[$PBS_ARRAYID-1]}} {run_args}'

if __name__ == '__main__': 
    run()


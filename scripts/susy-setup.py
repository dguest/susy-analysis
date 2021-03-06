#!/usr/bin/env python2.7
"""
Omega setup scripts
"""
import argparse, sys, os, math, random
from scharm import bullshit
from os.path import basename, splitext, dirname, isdir, isfile, join
import glob
import yaml

def run():
    config = get_config()
    subs = {'stack': setup_stack,
            'hadd': setup_hadd, 'fit': setup_fit, 'ws': setup_workspaces}
    subs[config.which](config)

def get_config():
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    subs = parser.add_subparsers(dest='which')

    stack_args = subs.add_parser('stack', description=setup_stack.__doc__)
    stack_args.add_argument('input_ntuples')
    stack_args.add_argument('-y', '--regions', required=True)
    stack_args.add_argument('-s', '--script', help='build this, ' + d,
                            default='crocosaurus.sh')
    stack_args.add_argument('-o', '--output-name',
                            default='batch/ntuple/ntuples.txt', help=d)
    stack_args.add_argument('-d','--hists-dir', default='hists', help=d)

    hadd_args = subs.add_parser('hadd', description=setup_hadd.__doc__)
    hadd_args.add_argument('input_dirs', nargs='+')
    hadd_args.add_argument('-o', '--textfile-name',
                           default='batch/hadddir/hadd.txt', help=d)
    hadd_args.add_argument('-s', '--script', help='build this, ' + d,
                           default='varzo.sh')
    hadd_args.add_argument('-d', '--hadd-dir', default='hadd-hists',
                           help=d)

    ws_args = subs.add_parser('ws', description=setup_workspaces.__doc__)
    ws_args.add_argument('kinematic_stat_dir')
    ws_args.add_argument('-c','--config-yaml', default='fits.yml', help=d)
    ws_args.add_argument('-s','--script',default='leaky-roof.sh',
                         help=d)
    ws_args.add_argument(
        '-n','--n-kin-subjobs', default=5, type=int, help=d)

    fit_args = subs.add_parser('fit', description=setup_fit.__doc__)
    fit_args.add_argument('root')
    fit_args.add_argument('-t','--text-file',action='store_true',
                          help='make steering text file')
    fit_args.add_argument(
        '-n','--fits-per-job', default=30, help=d, type=int)
    fit_args.add_argument('-s','--sql-db', action='store_true')

    return parser.parse_args(sys.argv[1:])

def _get_fit_config(file_name, counts, sig_stem='stop'):
    sig_pts = {k[1] for k in counts if sig_stem in k[1]}
    regions = {k[2] for k in counts}
    bgs = {k[1] for k in counts} - sig_pts - {'data',}
    if isfile(file_name):
        with open(file_name) as yml:
            conf_dict = yaml.load(yml)
        for reg in conf_dict.itervalues():
            reg_bg = set(reg['backgrounds'])
            reg_sr = reg['signal_region']
            reg_cr = set(reg['control_regions'])
            missing_bg = reg_bg - bgs
            if missing_bg:
                raise ValueError(
                    'no background {}'.format(', '.join(missing_bg)))
            if not reg_sr in regions:
                raise ValueError('no signal region {}'.format(reg_sr))
            missing_cr = reg_cr - regions
            if missing_cr:
                raise ValueError(
                    'no region {}'.format(', '.join(missing_cr)))
        return conf_dict

    def get_example(sr):
        crs = [str(r) for r in regions if not r.startswith('SR')]
        example = {
            'signal_region': str(sr),
            'control_regions': crs,
            'backgrounds': [str(b) for b in bgs],
            }
        return example
    probably_sr = [str(r) for r in regions if r.startswith('SR')]
    examples = {r:get_example(r) for r in probably_sr}
    with open(file_name,'w') as yml:
        yml.write(yaml.dump(examples))


def setup_workspaces(config):
    """
    Setup jobs to build workspaces for fitting. If no tag-config is given,
    uses the parent of kinematic_stat_dir.
    """
    from pyroot.fitter import CountDict
    counts = CountDict(config.kinematic_stat_dir, systematics=[])
    sig_pts = {k[1] for k in counts if 'stop' in k[1]}
    n_sp = len(sig_pts)

    fit_config = _get_fit_config(config.config_yaml, counts)
    if not fit_config:
        sys.exit('write example fit config ({}), exiting...'.format(
                config.config_yaml))

    n_fc = len(fit_config)
    n_jobs = n_fc * n_sp * config.n_kin_subjobs

    sub_dict = {
        'n_jobs': n_jobs,
        'out_dir': 'output/workspace',
        'in_dir': '.',
        'in_ext': '.txt',
        'walltime': '0:30:00',
        }
    submit_head = _get_submit_head(**sub_dict)
    submit_tmp = (
        'susy-histopt.py ms {} -y {} -n $(($PBS_ARRAYID-1)) -t {}')
    with open(config.script,'w') as the_job:
        the_job.write(submit_head + '\n')
        submit_line = submit_tmp.format(
            config.kinematic_stat_dir, config.config_yaml, n_jobs)
        the_job.write(submit_line + '\n')

def _build_fit_batch(root_dir, batch_path):
    n_jobs = 0
    model_name = 'stop_combined_meas_model.root'
    with open(batch_path, 'w') as bfile:
        for root, subdirs, files in bullshit.fast_walk(root_dir):
            if not model_name in files:
                continue
            n_jobs += 1
            full_path = os.path.join(root, model_name)
            bfile.write(full_path + '\n')
    return n_jobs

def setup_fit(config):
    """
    Sets up histfitter jobs. Will run one job for each leaf directory given
    under the root directory.

    Submits susy-histopt.py ul
    """
    batch_dir = 'batch/fits'
    batch_file = 'fit-paths.txt'

    if not isdir(batch_dir):
        os.makedirs(batch_dir)

    batch_path = os.path.join(batch_dir, batch_file)
    if config.text_file:
        n_fits = _build_fit_batch(config.root, batch_path)
    elif not isfile(batch_path):
        raise OSError("no batch file found at {}".format(batch_path))
    else:
        n_fits = 0
        with open(batch_path) as bfile:
            for line in bfile:
                if line.strip():
                    n_fits += 1
    sub_dict = {
        'n_jobs': int(math.ceil(n_fits / float(config.fits_per_job))),
        'out_dir': 'output/fit',
        'in_dir': batch_dir,
        'walltime': '30:00',
        }
    submit_head = _get_submit_head(**sub_dict)

    submit_line = 'susy-histopt.py ul {} -n $(($PBS_ARRAYID-1)) -m {}'.format(
        batch_path, config.fits_per_job)
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
    input_dirs = []
    # don't hadd some directories that aren't hists
    blacklist_dirs = ['event_list']
    for hadd_dir in config.input_dirs:
        if not isdir(hadd_dir):
            raise OSError("inputs must be directories")
        for root, dirs, files in os.walk(hadd_dir):
            # don't hadd some directories
            for bd in blacklist_dirs:
                if bd in dirs:
                    dirs.remove(bd)
            if files and dirs:
                raise OSError("confused: {} and {} in {}".format(
                    files, dirs, root))
            if files:
                input_dirs.append(root)

    sub_dict = {
        'n_jobs': len(input_dirs),
        'out_dir': 'output/hadd',
        'in_dir': dirname(config.textfile_name),
        'in_ext': '.txt',
        }
    submit_head = _get_submit_head(short=True, **sub_dict)
    line_args = {
        'routine': 'susy-hadd.py',
        'run_args': '--recursive --fast --output {}'.format(config.hadd_dir),
        }
    submit_line = '{routine} ${{files[$PBS_ARRAYID-1]}} {run_args}'
    run_line = submit_line.format(**line_args)

    with open(config.script, 'w') as out_script:
        out_script.write(submit_head)
        out_script.write('\n'.join(['files=('] + input_dirs + [')\n']))
        out_script.write(run_line + '\n')

def _get_meta(guess):
    if guess:
        if not isfile(guess):
            raise OSError('{} is not a file'.format(guess))
    candidates = glob.glob('*meta*.yml')
    if not candidates:
        raise OSError('no meta candidates found')
    if len(candidates) > 1:
        raise OSError(
            'too many meta candidates: ' + ', '.join(candidates))
    return candidates[0]

def setup_stack(config):
    """
    Sets up textfiles and shell script to run histograming via susy-stack.
    One histogram file will be created for each input ntuple.

    NOTE: makes use of random shuffling to put files in jobs,
    NOT deterministic!
    """
    all_files = list(_get_all_ntuples(config.input_ntuples))

    n_files = len(all_files)
    for file_n in xrange(n_files):
        file_name = '{}-{n}{}'.format(
            *splitext(config.output_name), n=file_n)
        if not isdir(dirname(file_name)):
            os.makedirs(dirname(file_name))
        with open(file_name, 'w') as out_file:
            out_file.writelines('\n'.join(all_files[file_n]) + '\n')

    sub_dict = {
        'n_jobs': n_files,
        'out_dir': 'output/stack',
        'in_dir': dirname(config.output_name),
        'in_ext': '.txt',
        }
    submit_head = _get_submit_head(**sub_dict)

    def get_runline(mode):
        line_args = {
            'routine': 'susy-stack.py',
            'run_args': '{} --mode {} -o {}'.format(
                config.regions, mode, config.hists_dir),
            }
        return _submit_line.format(**line_args) + '\n'

    with open(config.script, 'w') as out_script:
        out_script.write(submit_head)
        out_script.write(get_runline('nminus'))

def _get_all_ntuples(base_path, chunk_size=1e9):
    """
    returns random chunks of files in base_path, all of them should
    be under chunk_size in total size. Single files over chunk_size are
    added as their own chunk.
    """
    all_ntuples = []
    for root, dirs, files in os.walk(base_path):
        for fi in files:
            if fi.endswith('.root'):
                path = join(root, fi)
                all_ntuples.append((path, os.path.getsize(path)))
    random.shuffle(all_ntuples)

    tot_size = 0.0
    out_ntuples = []
    for path, size in all_ntuples:
        if tot_size + size > chunk_size and out_ntuples:
            yield out_ntuples
            tot_size = 0.0
            out_ntuples = []
        out_ntuples.append(path)
        tot_size += size
    if out_ntuples:
        yield out_ntuples

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

_submit_head = _short_subhead + "files=($(ls {in_dir}/*{in_ext} | sort))\n"

def _get_submit_head(n_jobs, out_dir, short=False, **args):
    default_args = dict(
        in_dir='.', in_ext='.txt', mem='4gb', walltime='00:06:00:00')
    default_args.update(args)
    subhead = _short_subhead if short else _submit_head
    return subhead.format(n_jobs=n_jobs, out_dir=out_dir, **default_args)




_submit_line = '{routine} ${{files[$PBS_ARRAYID-1]}} {run_args}'

if __name__ == '__main__':
    run()


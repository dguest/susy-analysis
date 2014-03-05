#!/usr/bin/env python2.7
"""
A simpler distillation script. No longer does multiprocessing or stores
lots of info in the meta file. 
"""
import argparse
import sys, os
from os.path import isfile, isdir, dirname
from os.path import join, splitext, basename, expanduser, split
from scharm import bullshit, schema
import re
import yaml
import warnings

_sys_choices = {'NONE', 'JESUP', 'JESDOWN', 'JER', 
                'METUP', 'METDOWN', 'METRES'}
def _get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('input_file', help='input file listing root inputs')
    parser.add_argument('meta', help='meta yaml file')
    parser.add_argument(
        '-o', '--output-dir', default='ntuples', 
        help='where the outputs go, ' + d)
    parser.add_argument(
        '-s', '--systematic', choices=_sys_choices, 
        help=d, default='NONE')
    parser.add_argument(
        '-c', '--calibration', default='~/calibration', help=d)
    parser.add_argument('-i', '--more-info', action='store_true')
    parser.add_argument('-e', '--all-events', action='store_true')
    parser.add_argument('--test', action='store_true')
    parser.add_argument('-p', '--build-prw', action='store_true')
    parser.add_argument('-a', '--aggressive', action='store_true')
    return parser.parse_args(sys.argv[1:])

def _get_ds_dict(meta_file_name, ds_key): 
    """
    Function to get this thing running even though there was a skimming bug
    Should remove ASAP
    """
    with open(meta_file_name) as yml: 
        yml_dict = yaml.load(yml)
        try: 
            return yml_dict[ds_key]
        except KeyError:
            warnings.warn('trying to fudge ds key to atlfast', stacklevel=2)
            return yml_dict['a' + ds_key[1:]]

def distill_d3pds(config): 
    """
    Main distill routine.
    """
    with open(config.input_file) as d3pd_file: 
        files = [l.strip() for l in d3pd_file.readlines()]
    if not files: 
        return 

    out_file = splitext(basename(config.input_file))[0] + '.root'
    ds_key = basename(splitext(out_file)[0]).split('-')[0]

    dataset = _get_ds_dict(config.meta, ds_key)
    flags, add_dict = _config_from_meta(dataset)
    add_dict['systematic'] = config.systematic

    add_dict.update(_get_cal_paths_dict(config))
    add_dict.update(_get_outputs(config, out_file))
        
    flags += _get_config_flags(config)
    if 'd' in flags: 
        if config.systematic != 'NONE' or config.build_prw: 
            return 

    _dump_settings(add_dict, flags)

    if config.test: 
        cut_counts = [('test',1)]
    else: 
        from scharm.distiller import cutflow
        cut_counts = cutflow.cutflow(
            input_files=files, 
            flags=flags, 
            cutflow='NOMINAL', 
            **add_dict)

    _write_cutflow(cut_counts, add_dict['output_ntuple'])

def _write_cutflow(cut_counts, output_ntuple): 
    """
    Write out a yaml file showing the cuts passed
    """
    ntup_dir, ntup_name = split(output_ntuple)
    cf_name = '{}_counts.yml'.format(splitext(ntup_name)[0])
    counts_dir = join(ntup_dir.rsplit('/',1)[0], 'cutflows')
    bullshit.make_dir_if_none(counts_dir)
    counts_path = join(counts_dir, cf_name)
    list_counts = [list(c) for c in cut_counts]
    with open(counts_path,'w') as out_yml: 
        out_yml.write(yaml.dump(list_counts))

def _get_cal_paths_dict(config): 
    calibration_dir = expanduser(config.calibration)
    call_paths = dict(
        btag_cal_file=join(calibration_dir, 'BTagCalibration.env'), 
        grl=join(calibration_dir, 'grl.xml'), 
        pu_lumicalc=join(calibration_dir, 'pu_lumicalc.root')
        )
    if not config.build_prw: 
        call_paths['pu_config'] = join(
            calibration_dir, 'pu_config.prw.root')

    for entry, cal_file in call_paths.iteritems(): 
        if not isfile(cal_file): 
            raise OSError("can't find '{}' at {}".format(entry, cal_file))

    call_paths['cal_dir'] = calibration_dir
    return call_paths

def _get_outputs(config, out_file): 
    """
    various output configurations
    """
    char = out_file[0]
    if char not in schema.stream_schema: 
        raise ValueError("stream not recognized: {}".format(out_file))

    sys_dir = config.systematic.lower()
    stream_dir = schema.stream_schema[char]

    def make_output(replacement): 
        out_dir = join(config.output_dir, stream_dir, sys_dir, replacement)
        bullshit.make_dir_if_none(out_dir)
        return join(out_dir, out_file)

    return {'mumet_output_ntuple': make_output('mumet'), 
            'output_ntuple': make_output('normal'),
            'leptmet_out_ntuple': make_output('leptmet')}

def _dump_settings(settings_dict, flags): 
    print "running with flags '{}'".format(flags)
    set_width = max(len(str(n)) for n in settings_dict)
    for name, value in settings_dict.iteritems(): 
        print '{n:{w}}: {v}'.format(n=name, w=set_width, v=value)

def _config_from_meta(dataset): 
    flags = ''
    full_name = dataset['full_name']
    if full_name.startswith('data'): 
        flags += 'd'
    elif _is_atlfast(full_name): 
        flags += 'f'
    overlap = dataset.get('overlap',{})
    add_dict = dict(
        boson_pt_max_mev = float(overlap.get('sherpa_boson',-1.0))*1e3, 
        truth_met_max_mev = float(overlap.get('truth_met', -1.0))*1e3, 
        )
    if _is_sherpa_ew(dataset): 
        flags += 'p'            # do boson pt reweighting
    return flags, add_dict

def _get_config_flags(config):
    flags = ''
    if config.more_info: 
        flags += 'i'           # save sparticle id
    if config.all_events: 
        flags += 'e'            # disables skimming
    if sys.stdin.isatty(): 
        flags += 'v'            # verbose
    if config.aggressive: 
        flags += 'a'            # remove bad files and try again
    return flags

# --- various smaller checks

def _is_atlfast(sample): 
    sim_re = re.compile('\.e[0-9]+_([as])[0-9]+_')
    try: 
        char = sim_re.search(sample).group(1)
    except AttributeError: 
        raise ValueError("can't find type of reco in {}".format(sample))
    if char == 'a': 
        return True
    elif char == 's': 
        return False
    else: 
        raise ValueError(
            "not sure what kind of sample '{}' is".format(sample))

def _is_sherpa_ew(dataset): 
    ew_re = re.compile('\.[Ss]herpa_.*(W(e|mu|tau)nu|Z(e|mu|tau|nu){2})')
    full_name = dataset['full_name']
    pt = dataset['physics_type']
    ew_matched = ew_re.search(full_name)
    type_matched = bool(pt) and any(t in pt for t in {'Wjets','Zjets'})
    if bool(ew_matched) != type_matched: 
        prob = (
            "can't make sense of physics type '{}' with name '{}', "
            "ew match = {}, type match = {}").format(
            pt, full_name, bool(ew_matched), 
            type_matched)
        raise ValueError(prob)
    return bool(ew_matched)

def run(): 
    args = _get_config()
    distill_d3pds(args)

# ------ not used right now... revisit ----

def _setup_prw(config): 
    # right now we're not using this... not sure if it should be here
    if config.build_prw:        
        prw_dir = 'pileup-reweighting'
        bullshit.make_dir_if_none(prw_dir)
        prw_file = join(prw_dir, '{}.prw{}'.format(*splitext(out_file)))
        # flags += 'u'            # turn on pu file generation 
        add_dict['pu_config'] = prw_file
        out_path = ''           # disable output trees
    else: 
        bullshit.make_dir_if_none(config.output_dir)


if __name__ == '__main__': 
    run()

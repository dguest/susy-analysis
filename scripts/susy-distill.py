#!/usr/bin/env python2.7
"""
A simpler distillation script. No longer does multiprocessing or stores
lots of info in the meta file. 
"""
import argparse
import sys, os
from os.path import isfile, isdir
from os.path import join, splitext, basename, expanduser
from stop import meta
import re
import yaml

def _get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('input_file', help='input file listing root inputs')
    parser.add_argument('meta', help='meta yaml file')
    parser.add_argument(
        '-o', '--output-dir', default='whiskey', 
        help='where the outputs go, ' + d)
    parser.add_argument(
        '-s', '--systematic', choices={'NONE', 'JESUP', 'JESDOWN', 'JER'}, 
        help=d, default='NONE')
    parser.add_argument(
        '-c', '--calibration', default='~/calibration', help=d)
    parser.add_argument('-i', '--more-info', action='store_true')
    parser.add_argument('-e', '--all-events', action='store_true')
    parser.add_argument('--test', action='store_true')
    return parser.parse_args(sys.argv[1:])


def distill_d3pds(config): 

    with open(config.input_file) as d3pd_file: 
        files = [l.strip() for l in d3pd_file.readlines()]
    out_file = splitext(basename(config.input_file))[0] + '.root'

    calibration_dir = expanduser(config.calibration)
    if not files: 
        return 
    if not isdir(config.output_dir): 
        os.makedirs(config.output_dir)

    meta_lookup = meta.DatasetCache(config.meta)
    ds_key = basename(splitext(out_file)[0]).split('-')[0]
    dataset = meta_lookup[ds_key]
    flags, add_dict = _config_from_meta(dataset)
    add_dict.update(dict(
            systematic=config.systematic, 
            btag_cal_file=join(calibration_dir, 'BTagCalibration.env'), 
            cal_dir=calibration_dir, 
            grl=join(calibration_dir, 'grl.xml'), 
            ))

    out_path = join(config.output_dir, out_file)

    if config.more_info: 
        flags += 'i'           # save sparticle id
    if config.all_events: 
        flags += 'e'            # disables skimming
    if 'd' in flags: 
        if config.systematic != 'NONE': 
            return 

    if sys.stdin.isatty(): 
        flags += 'v'            # verbose
    else: 
        print 'running {} with flags {}'.format(config.input_file, flags)
        _dump_settings(add_dict)

    if config.test: 
        cut_counts = [('test',1)]
    else: 
        from stop.distiller import cutflow
        cut_counts = cutflow.cutflow(
            input_files=files, 
            flags=flags, 
            output_ntuple=out_path, 
            cutflow='NOMINAL', 
            **add_dict)

    counts_path = splitext(out_path)[0] + '_counts.yml'
    list_counts = [list(c) for c in cut_counts]
    with open(counts_path,'w') as out_yml: 
        out_yml.write(yaml.dump(list_counts))

def _dump_settings(settings_dict): 
    set_width = max(len(str(n)) for n in settings_dict)
    for name, value in settings_dict.iteritems(): 
        print '{n:{w}}: {v}'.format(n=name, w=set_width, v=value)

def _config_from_meta(dataset): 
    flags = ''
    if dataset.is_data: 
        flags += 'd'
    elif _is_atlfast(dataset.full_name): 
        flags += 'f'
    overlap = dataset.overlap
    add_dict = dict(
        boson_pt_max_mev = float(overlap.get('sherpa_boson',-1.0))*1e3, 
        truth_met_max_mev = float(overlap.get('truth_met', -1.0))*1e3, 
        )
    if add_dict['truth_met_max_mev'] < 0: 
        flags += 'h'            # hack for old skims (no truth met branch)
    return flags, add_dict

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

def _needs_overlap_removal(sample): 
    overlaping_sherpa_finder = re.compile('MassiveCBPt0_')
    sherpa_pt_range_finder = re.compile('Pt[0-9]+_[0-9]+')
    if overlaping_sherpa_finder.search(sample): 
        if sherpa_pt_range_finder.search(sample): 
            raise IOError( 
                "can't determine if {} needs pt overlap removal".format(
                    sample))
        return True
    return False

def _is_data(sample): 
    return sample.startswith('data')

def _ntuple_name_from_ds_name(ds_name): 
    bname = basename(ds_name)
    fields = bname.split('.')
    try: 
        type_index = fields.index('mc12_8TeV')
    except ValueError: 
        raise ValueError("can't find type string in {}".format(bname))
    dsid = fields[type_index + 1]
    sim_name = fields[type_index + 2].split('_')[0]
    return '{}-{}.root'.format(sim_name, dsid)

def run(): 
    args = _get_config()
    distill_d3pds(args)

if __name__ == '__main__': 
    run()

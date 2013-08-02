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
    parser.add_argument('-f', '--filter-events', action='store_true')
    parser.add_argument('--test', action='store_true')
    return parser.parse_args(sys.argv[1:])


def distill_d3pds(config): 

    with open(config.input_file) as d3pd_file: 
        files = [l.strip() for l in d3pd_file.readlines()]
    out_file = splitext(basename(config.input_file))[0] + '.root'

    calibration_dir = expanduser(config.calibration)
    btag_env = join(calibration_dir, 'BTagCalibration.env')
    grl = join(calibration_dir, 'grl.xml')
    if not files: 
        return 
    if not isdir(config.output_dir): 
        os.makedirs(config.output_dir)
    meta_lookup = meta.DatasetCache(config.meta)

    ds_key = basename(splitext(out_file)[0]).split('-')[0]

    out_path = join(config.output_dir, out_file)
    
    flags = ''
    if config.more_info: 
        flags += 'i'           # save sparticle id
    if not config.filter_events: 
        flags += 'e'            # disables skimming
    if meta_lookup[ds_key].is_data: 
        flags += 'd'
        if config.systematic != 'NONE': 
            return 
    elif _is_atlfast(meta_lookup[ds_key].full_name): 
        flags += 'f'
    if _needs_overlap_removal(meta_lookup[ds_key].full_name): 
        flags += 'h'

    if sys.stdin.isatty(): 
        flags += 'v'            # verbose
    else: 
        print 'running {} with flags {}'.format(config.input_file, flags)

    if config.test: 
        cut_counts = [('test',1)]
    else: 
        from susy import cutflow
        cut_counts = cutflow.cutflow(
            input_files=files, 
            flags=flags, 
            output_ntuple=out_path, 
            grl=grl,
            systematic=config.systematic, 
            btag_cal_file=btag_env, cal_dir=calibration_dir, 
            cutflow='NOMINAL')

    counts_path = splitext(out_path)[0] + '_counts.yml'
    list_counts = [list(c) for c in cut_counts]
    with open(counts_path,'w') as out_yml: 
        out_yml.write(yaml.dump(list_counts))

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

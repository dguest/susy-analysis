#!/usr/bin/env python3.3
"""
Convert the output from `ami dataset info <full ds name>` to the meta format.
"""

import yaml, re, sys
from argparse import ArgumentParser

def run():
    parser = ArgumentParser(description=__doc__)
    parser.add_argument('ami_output')
    parser.add_argument(
        '-o', '--output-meta', help='default: stdout')
    args = parser.parse_args()
    make_meta(args.ami_output, args.output_meta)

# __________________________________________________________________________
# schema to read out info / convert to meta format we use
_newfile_key = 'Dataset Parameters'
_skip_keys = {_newfile_key, 'Extra Parameters'}
_dsnum_key = 'datasetNumber'
_atlfinder = re.compile('(_a([0-9])+)+')
def _make_fb(xsec_nb):
    """convert an xsec string to the proper float"""
    return float(xsec_nb) * 1e6
_ami_to_meta_schema = {
    'logicalDatasetName': (str, 'full_name'),
    'totalEvents': (int, 'n_expected_entries'),
    'approx_GenFiltEff': (float, 'filteff'),
    'approx_crossSection': (_make_fb, 'total_xsec_fb'),
    }


def _get_char(version):
    """get the charicter code for a dataset"""
    if _atlfinder.search(version):
        return 'a'
    raise ValueError('doesnt work with not-atlfast')

# ___________________________________________________________________________
# textfile parsers

def _info_iter(input_file_name):
    """generator for info dicts"""
    with open(input_file_name) as infile:
        for ds in _text_dicter(infile):
            char = _get_char(ds['version'])
            dsid = ds['datasetNumber']
            out_dict = {}
            for oldkey, val in ds.items():
                convt, key = _ami_to_meta_schema.get(oldkey, (None, None))
                if convt:
                    out_dict[key] = convt(val)
            yield char + dsid, out_dict

def _text_dicter(infile):
    """split an input file into fields"""
    out_dict = {}
    for line in infile:
        if out_dict and line.strip() == _newfile_key:
            yield out_dict
            out_dict = {}

        if line.strip() in _skip_keys: continue
        sline = line.split(None,1)
        if len(sline) >= 2:
            if sline[0] in out_dict:
                raise ValueError(
                    'found dup: {} in {}'.format(
                        sline, out_dict.get('logicalDatasetName', out_dict)))
            out_dict[sline[0]] = sline[1].strip()

# __________________________________________________________________________
# top level function

def make_meta(ami_txt_file, output_yml_file):
    """top level function"""
    from os.path import splitext
    meta_dict = {}
    for ds_key, ds in _info_iter(ami_txt_file):
        # WARNING: this is a hack, assumes everything is signal
        ds['physics_type'] = 'signal'
        ds['kfactor'] = 1.0
        ds['meta_sources'] = ['ami']
        meta_dict[ds_key] = ds
    out = open(output_yml_file, 'w') if output_yml_file else sys.stdout
    out.write(yaml.dump(meta_dict))

if __name__ == '__main__':
    run()

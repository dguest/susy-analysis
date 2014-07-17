#!/usr/bin/env python3.3
"""Add qcd, other systematics I'm not computing to the fit inputs"""

import argparse
import yaml

def get_config():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('fit_inputs')
    return parser.parse_args()

def run():
    args = get_config()
    with open(args.fit_inputs, 'r+') as yml:
        yields_dict = yaml.load(yml)
        _add_qcd(yields_dict)
        yml.truncate(0)
        yml.seek(0)
        yml.write(yaml.dump(yields_dict))

def _add_qcd(yields_dict):
    sr_qcd = {
        'signal_mct150': 0.34,
        'signal_mct200': 0.21,
        'signal_mct250': 0.05,
        }
    for sr, count in sr_qcd.items():
        yields_dict['nominal_yields'][sr]['QCD'] = [count, count]

if __name__ == '__main__':
    run()

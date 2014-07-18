#!/usr/bin/env python3.3
"""Add qcd, other systematics I'm not computing to the fit inputs"""

import argparse
import yaml

_nom_key = 'nominal_yields'
_yld_key = 'yield_systematics'
_rel_key = 'relative_systematics'

def get_config():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('fit_inputs')
    return parser.parse_args()

def run():
    args = get_config()
    with open(args.fit_inputs, 'r+') as yml:
        yields_dict = yaml.load(yml)
        _add_qcd(yields_dict)
        _add_mettrig(yields_dict)
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
        # give the qcd 100% statistical uncertainty
        yields_dict['nominal_yields'][sr]['QCD'] = [count, count]

def _add_mettrig(yields_dict, error=0.01):
    """adds a 1% uncertainty to all the signal regions"""
    nom = yields_dict[_nom_key]
    srs = [x for x in nom if x.startswith('signal_')]
    relsyst = yields_dict.setdefault(_rel_key, {})
    mettrig = relsyst.setdefault('mettrig', {})
    for sr in srs:
        mettrig[sr] = [1 - error, 1 + error]

if __name__ == '__main__':
    run()

#!/usr/bin/env python3.3
"""Add qcd, other systematics I'm not computing to the fit inputs"""

import argparse
import yaml

_nom_key = 'nominal_yields'
_yld_key = 'yield_systematics'
_rel_key = 'relative_systematics'

_misc_syst_help = 'update with an arbitrary dictionary (will overwrite)'

def get_config():
    d = 'default: %(default)s'
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('fit_inputs')
    parser.add_argument('--met-trig-error', type=float, default=0.02, help=d)
    parser.add_argument('--misc-syst', help=_misc_syst_help)
    return parser.parse_args()

def run():
    args = get_config()
    with open(args.fit_inputs, 'r+') as yml:
        yields_dict = yaml.load(yml)
        _add_qcd(yields_dict)
        _add_mettrig(yields_dict, error=args.met_trig_error)
        _add_misc(yields_dict, args.misc_syst)
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
        if sr not in yields_dict[_nom_key]:
            continue
        # give the qcd 100% statistical uncertainty
        yields_dict[_nom_key][sr]['QCD'] = [count, count]

def _add_mettrig(yields_dict, error=0.01):
    """
    adds an uncertainty to all the signal regions to account for the trigger
    error.
    """
    nom = yields_dict[_nom_key]
    srs = [x for x in nom if x.startswith('signal_')]
    relsyst = yields_dict.setdefault(_rel_key, {})
    mettrig = relsyst.setdefault('mettrig', {})
    for sr in srs:
        mettrig[sr] = [1 - error, 1 + error]

def _add_misc(yields_dict, misc):
    """
    Update the dictionary with the file that `misc` points to. For safety
    only allow the relative systematics to be updated.
    """
    if not misc:
        return
    with open(misc) as yml:
        new_dict = yaml.load(yml)
    yields_dict[_rel_key].update(new_dict[_rel_key])

if __name__ == '__main__':
    run()

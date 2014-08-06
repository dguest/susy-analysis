#!/usr/bin/env python3.3
"""Add qcd, other systematics I'm not computing to the fit inputs"""

import argparse
import yaml
import bisect

_nom_key = 'nominal_yields'
_yld_key = 'yield_systematics'
_rel_key = 'relative_systematics'
_sig_key = 'scharm-'

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
        _add_signal_isr(yields_dict)
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

def _add_signal_isr(yields_dict):
    """Add an isr systematic (taken from sbottom) to the signal points"""

    # pulled from sbottom
    dm_vs_isr_syst = [
        (0, 0.287),
        (10, 0.076),
        (30, 0.079),
        (50, 0.112),
        ]

    dm_vals, ret_vals = zip(*dm_vs_isr_syst)
    def get_isr(sig_name):
        prefix, msusy, mlsp = sig_name.split('-')
        dm = int(msusy) - int(mlsp)
        idx = bisect.bisect(dm_vals, dm) - 1
        return ret_vals[idx]

    nomv = yields_dict[_nom_key].values()
    all_sigp = {x for y in nomv for x in y if x.startswith(_sig_key)}
    all_sr = {x for x in yields_dict[_nom_key] if x.startswith('signal_')}

    isr_syst = {}
    for sigp in all_sigp:
        isr_val = get_isr(sigp)
        variation = [1.0 - isr_val, 1.0 + isr_val]
        for sr in all_sr:
            isr_syst.setdefault(sr,{})[sigp] = variation

    yields_dict[_rel_key]['signal_isr'] = isr_syst

if __name__ == '__main__':
    run()
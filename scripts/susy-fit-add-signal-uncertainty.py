#!/usr/bin/env python3
"""Add signal uncertainty to CLs file"""

import argparse
import yaml

_default_systematics = [
    'jes','jer','jvf','pu','el','mu','met','metres','eglow','mscale','egzee',
    'c','b','u','t','mettrig','signal_isr']
_default_regions = ['signal_mct150']

def get_args():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('fit_inputs_file')
    parser.add_argument('cls_file')
    parser.add_argument('-s','--systematics', default=_default_systematics,
                        nargs='*')
    parser.add_argument('-r','--regions', default=_default_regions, nargs='*')
    return parser.parse_args()

def run():
    args = get_args()
    with open(args.fit_inputs_file,'r') as fit_inputs:
        fit_input_dict = yaml.load(fit_inputs)

    for region in args.regions:
        for point in _get_signal_points(fit_input_dict):
            rel_unct = _get_relative_uncertainty(
                fit_input_dict, args.systematics, region, point)
            print(rel_unct)

def _get_relative_uncertainties(fit_input_dict, systematics, region, point):
    base_yield = fit_input_dict['nominal_yields'][region][point][0]
    raw_systs = fit_input_dict['yield_systematics']
    raw_rel_systs = fit_input_dict['relative_systematics']

    sym_systs  = _get_sym_systs2(base_yield, raw_systs, region, point)
    asym_systs = _get_asym_systs2(base_yield, raw_systs, region, point)
    rel_systs = _get_rel_systs2(raw_rel_systs, region, point)

    all_systs = sym_systs.copy()
    all_systs.update(asym_systs)
    all_systs.update(rel_systs)
    return all_systs.keys()

# _________________________________________________________________________
# funcs to get yields

UP, DOWN = 'up', 'down'
def _get(region_dict, region, point, base):
    return region_dict.get(region,{}).get(point,[base])[0]

def _get_sym_systs2(base_yield, raw_systs, region, point):
    rel_systs = {}
    for syst, regdic in raw_systs:
        if syst.endswith(UP) or syst.endswith(DOWN):
            continue
        syst_yield = _get(regdic, region, point, base_yield)
        rel_systs[syst] = ((syst_yield - base_yield) / base_yield)**2
    return rel_systs

def _get_asym_systs2(base_yield, raw_systs, region, point):
    rel_systs = {}
    for syst, regdic in raw_systs:
        if not syst.endswith(UP):
            continue
        base_key = syst[:-len(UP)]
        down_key = base_key + DOWN
        if down_key not in raw_systs:
            raise KeyError("can't find down part for {}".format(base_key))
        up = _get(regdic, region, point, base_yield)
        down = _get(raw_systs[down_key], region, point, base_yield)
        rel_systs[syst[:-len(UP)]] = ((up - down) / base_yield)**2
    return rel_systs

def _get_rel_systs2(raw_rel_systs, region, point):
    rel_systs = {}
    for syst, regdic in raw_rel_systs:
        ptdic = raw_rel_systs.get(region)
        if ptdic is None:
            continue
        # two ways to format things here, first is for each point
        try:
            down, up = ptdic.get(point, [1,1])
        except AttributeError:
            down, up = ptdic
        rel_systs[syst] = (up - down)**2
    return rel_systs

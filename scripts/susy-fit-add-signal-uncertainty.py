#!/usr/bin/env python3
"""Add signal uncertainty to CLs file"""

import argparse
import yaml
import re
import sys

_default_systematics = [
    'jes','jer','jvf','pu','el','mu','met','metres','eglow','mscale','egzee',
    'c','b','u','t','mettrig','signal_isr']
_default_regions = ['signal_mct150','signal_mct200','signal_mct250']

_signal_regex = re.compile('\w+-([0-9]+)-([0-9]+)')

# input yaml file keys
UP, DOWN = 'up', 'down'
NOMINAL_YIELDS = 'nominal_yields'
YIELD_SYSTEMATICS = 'yield_systematics'
RELATIVE_SYSTEMATICS = 'relative_systematics'
# output names
OUTPUT_ATTRIBUTE_NAME = 'sig_unct'

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
    with open(args.cls_file) as cls:
        cls_dict = CLsDict(yaml.load(cls))

    for region in args.regions:
        for point, ms, ml in _yield_signal_points(fit_input_dict):
            rel_unct = _get_relative_uncertainty(
                fit_input_dict, args.systematics, region, point)

            # HACK to rename the regions names to config names
            config_name = region.replace('signal_','')

            outpoint = cls_dict.setdefault( (config_name, ms, ml), {})
            outpoint[OUTPUT_ATTRIBUTE_NAME] = rel_unct

    sys.stdout.write(yaml.dump(cls_dict.get_out_format()))

class CLsDict(dict):
    """keyed by (region, mstop, mlsp)"""
    def __init__(self, dic_from_yaml):
        for region, fits in dic_from_yaml.items():
            for fit in fits:
                msch, mlsp = fit['scharm_mass'], fit['lsp_mass']
                self[region, msch, mlsp] = fit

    def get_out_format(self):
        out = {}
        for (region, ms, ml), point in self.items():
            config_list = out.setdefault(region, [])
            config_list.append(point)
        return out

def _get_relative_uncertainty(fit_input_dict, systematics, region, point):
    try:
        base_yield = fit_input_dict[NOMINAL_YIELDS][region][point][0]
    except KeyError:
        return 0.0
    raw_systs = fit_input_dict[YIELD_SYSTEMATICS]
    raw_rel_systs = fit_input_dict[RELATIVE_SYSTEMATICS]

    sym_systs  = _get_sym_systs2(base_yield, raw_systs, region, point)
    asym_systs = _get_asym_systs2(base_yield, raw_systs, region, point)
    rel_systs = _get_rel_systs2(raw_rel_systs, region, point)

    all_systs = sym_systs.copy()
    all_systs.update(asym_systs)
    all_systs.update(rel_systs)
    total_square_uncertainty = 0.0
    for syst in systematics:
        total_square_uncertainty += all_systs[syst]
    return total_square_uncertainty**0.5

# _________________________________________________________________________
# funcs to get yields

def _get(region_dict, region, point, base):
    return region_dict.get(region,{}).get(point,[base])[0]

def _get_sym_systs2(base_yield, raw_systs, region, point):
    rel_systs = {}
    for syst, regdic in raw_systs.items():
        if syst.endswith(UP) or syst.endswith(DOWN):
            continue
        syst_yield = _get(regdic, region, point, base_yield)
        rel_systs[syst] = ((syst_yield - base_yield) / base_yield)**2
    return rel_systs

def _get_asym_systs2(base_yield, raw_systs, region, point):
    rel_systs = {}
    for syst, regdic in raw_systs.items():
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
    for syst, regdic in raw_rel_systs.items():
        ptdic = regdic.get(region)
        if ptdic is None:
            continue
        # two ways to format things here, first is for each point
        try:
            down, up = ptdic.get(point, [1,1])
        except AttributeError:
            down, up = ptdic
        rel_systs[syst] = (up - down)**2
    return rel_systs

# _________________________________________________________________________
# utilities
def _yield_signal_points(fit_input_dict):
    regions_dict = fit_input_dict[NOMINAL_YIELDS]
    all_proc = set()

    for proc_dict in regions_dict.values():
        all_proc |= set(proc_dict)

    for proc in all_proc:
        match = _signal_regex.search(proc)
        if match:
            ms, ml = tuple(int(x) for x in match.group(1,2))
            yield proc, ms, ml

if __name__ == '__main__':
    run()

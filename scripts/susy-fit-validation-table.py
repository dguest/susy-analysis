#!/usr/bin/env python3
"""Dump a yields table for validation regions"""

import argparse, sys
import yaml
from collections import Counter
from scharm.limits import limitsty

_nom_key = 'nominal_yields'
_yld_key = 'yield_systematics'
_rel_key = 'relative_systematics'
_sig_key = 'scharm-'

# None means don't show that process
_proc_heads = {'QCD': None, 'data': None}
# some are renamed for simplicity
_renamed_procs = {
    'singleTop': 'top',
    }

def get_config():
    d = 'default: %(default)s'
    def_regions = ['signal_mct150', 'signal_mct200', 'signal_mct250']
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('fit_inputs')
    parser.add_argument('fit_parameters')
    parser.add_argument(
        '-r', '--regions', nargs='*', default=def_regions, help=d)
    return parser.parse_args()

def run():
    args = get_config()
    with open(args.fit_inputs) as yml:
        yields_dict = yaml.load(yml)
    with open(args.fit_parameters) as yml:
        parameters = yaml.load(yml)

    raw_nom_yields = yields_dict[_nom_key]
    nom_yields = _combine_yields(raw_nom_yields)
    syst_yields = _combine_systs(yields_dict[_yld_key], raw_nom_yields)

    mus = _get_pars(parameters, prefix='mu_')
    data_list = _get_data_list(nom_yields, args.regions)
    mc_expt, tot = _get_mc_expected_lists(nom_yields, args.regions)
    mc_fit, tot_fit = _get_mc_expected_lists(nom_yields, args.regions, mus)
    reg_names = _get_regnames(args.regions)
    out_lines = [
        [
            [r'{\bf channel}'] + reg_names,
            ],
        [
            data_list,
            ],
        [
            tot_fit,
            ],
        mc_fit,
        [
            tot,
            ],
        mc_expt,
        ]
    _dump_table(out_lines)

def _combine_yields(noms):
    """combine the yields according to how they are fit"""
    combined = {}
    for region, procdic in noms.items():
        comb_procs = combined.setdefault(region, {})
        for proc, vals in procdic.items():
            newproc = _renamed_procs.get(proc, proc)
            if newproc not in comb_procs:
                comb_procs[newproc] = vals
            else:
                newvals = comb_procs[newproc]
                newvals[0] += vals[0]
                if len(newvals) > 1:
                    newvals[1] = (vals[1]**2 + newvals[1]**2)**0.5

    return combined

def _combine_systs(systs, nominal):
    comb_systs = {}
    # start by looping through all the systematics
    for syst, regdic in systs.items():
        comb_regs = comb_systs.setdefault(syst, {})
        # start looping over nominal here, since we want a systematic
        # for any nominal leaf
        for region, procdic in nominal.items():
            comb_procs = comb_regs.setdefault(region, {})
            for proc, nomvals in procdic.items():
                newproc = _renamed_procs.get(proc, proc)
                # pull the region systematic yield, or just use the nominal
                sys_val = regdic.get(region,{}).get(proc,nomvals)[0]
                print(sys_val)
                if newproc not in comb_procs:
                    comb_procs[newproc] = [sys_val]
                else:
                    comb_procs[newproc][0] += sys_val
    return comb_systs

def _get_regnames(regions):
    reg_names = []
    for region in regions:
        reg_names.append(limitsty.reg_names.get(region, region))
    return reg_names

def _dump_table(out_lines, out=sys.stdout):
    lens = [len(x) for y in out_lines for x in y]
    if not max(lens) == min(lens):
        raise ValueError('line lengths: {}'.format(lens))
    tablen = max(lens)
    out.write(r'\begin{tabular}{|' + '|'.join('c'*tablen) + '|}' '\n')
    for sec in out_lines:
        out.write(r'\hline' '\n')
        for line in sec:
            out.write(' & '.join(line) + r' \\' '\n')
    out.write(r'\hline' '\n')
    out.write(r'\end{tabular}' '\n')

def _get_pars(parameters_dict, prefix='mu_'):
    """returns a dict of (fit parameter, error), keyed by the parameter"""
    pars = {}
    for par, val in parameters_dict.items():
        if par.startswith(prefix):
            afit = val['after']
            pars[par[len(prefix):]] = (afit['value'], afit['error'])
    return pars

def _get_procs(regions):
    """get all procs in the (nested) dict `regions`"""
    procs = set()
    for procdic in regions.values():
        for proc in procdic:
            if not proc.startswith(_sig_key):
                procs.add(proc)
    return procs

_nan = float('nan')
def _get_mc_expected_lists(nom_yields, region_names, mu_pars={}):
    """
    returns nested lists of strings (for formatting yields table)
    """
    regions = {x:nom_yields[x] for x in region_names}
    procs = _get_procs(regions)
    linehead = 'MC exp.' if not mu_pars else 'Fitted'
    lines = []
    totals = Counter()
    for proc in sorted(procs):
        spec_head = _proc_heads.get(proc, linehead)
        if spec_head is None:
            continue
        line_entries = ['{} {} events'.format(spec_head, proc)]
        for region in region_names:
            proc_count = regions[region].get(proc, [0,0])[0]
            norm = mu_pars.get(proc, (1.0, _nan ))
            proc_count *= norm[0]
            line_entries.append('{:.1f}'.format(proc_count))
            totals[region] += proc_count
        lines.append(line_entries)
    total_list = ['{} total'.format(linehead)]
    for region in region_names:
        total_list.append('{:.1f}'.format(totals[region]))
    return lines, total_list

def _get_data_list(nom_yields, region_names):
    regions = {x:nom_yields[x] for x in region_names}
    line_entries = ['Observed events']
    for region in region_names:
        count = regions[region].get('data',(None, None))[0]
        dat_string = '---' if count is None else '{:.0f}'.format(count)
        line_entries.append(dat_string)
    return line_entries

if __name__ == '__main__':
    run()

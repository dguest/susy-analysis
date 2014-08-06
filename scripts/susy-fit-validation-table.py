#!/usr/bin/env python3.3
"""Dump a yields table for validation regions"""

import argparse, sys
import yaml
from collections import Counter

_nom_key = 'nominal_yields'
_yld_key = 'yield_systematics'
_rel_key = 'relative_systematics'
_sig_key = 'scharm-'

# None means don't show that process
_proc_heads = {'QCD': None, 'data': None}

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

    nom_yields = yields_dict[_nom_key]

    mus = _get_pars(parameters, prefix='mu_')
    data_list = _get_data_list(nom_yields, args.regions)
    mc_expt, tot = _get_mc_expected_lists(nom_yields, args.regions)
    mc_fit, tot_fit = _get_mc_expected_lists(nom_yields, args.regions, mus)
    out_lines = [
        [
            [r'{\bf channel}'] + args.regions,
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

def _dump_table(out_lines, out=sys.stdout):
    lens = [len(x) for y in out_lines for x in y]
    if not max(lens) == min(lens):
        raise ValueError('line lengths: {}'.format(lens))
    tablen = max(lens)
    out.write(r'\begin{tabular}{' + '|'.join('c'*tablen) + '}' '\n')
    for sec in out_lines:
        out.write(r'\hline' '\n')
        for line in sec:
            out.write(' & '.join(line) + r'\\' '\n')
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

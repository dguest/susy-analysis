#!/usr/bin/env python3
"""Make custom yield tables. Narrow version by default."""

_yields_help = "Yields table from susy-fit-yields. Also allows piping in."

import argparse
import sys, yaml

def run():
    args = _get_args()
    yields = _get_yaml(args.yields)
    enc = _encode(yields)
    if args.wide:
        sys.stdout.write(_wide_table.format(**enc))
        return
    sys.stdout.write(_narrow_table.format(**enc))


def _get_args():
    """input parser"""
    def bl(help_string):
        return dict(action='store_true', help=help_string)
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('yields', help=_yields_help, nargs='?')
    plot_type = parser.add_mutually_exclusive_group()
    plot_type.add_argument('-w', '--wide', **bl('wide version'))
    return parser.parse_args()

def _get_yaml(file_name):
    if not file_name and not sys.stdin.isatty():
        return yaml.load(sys.stdin)

    with open(file_name) as pars:
        return yaml.load(pars)

def _number_with_unct(numlist):
    if numlist[0] < 0.1:
        return '$< 0.1$'
    sigfig = 1 if numlist[0] < 10 else 0
    return r'${:.{s}f} \pm {:.{s}f}$'.format(*numlist, s=sigfig)

def _num(numlist):
    if numlist[0] < 0.1:
        return '$< 0.1$'
    sigfig = 1 if numlist[0] < 10 else 0
    return r'{{\braketsize $({:.{s}f})$}}'.format(numlist[0], s=sigfig)

_reg_dic = {'cr_' + c: c for c in 'wtz'}
_reg_dic.update({'signal_mct{}0'.format(x): str(x) for x in [15,20,25]})
_proc_dic = {
    'Wjets': 'w',
    'Zjets': 'z',
    'top': 't',
    'other': 'o',
}
def _encode(yields):
    coded = {}
    for region, procs in yields.items():
        rchar = _reg_dic.get(region)
        if not rchar:
            continue
        for proc, prov in procs.items():
            pchar = _proc_dic.get(proc)
            if pchar:
                code = pchar + rchar
                coded[code] = _number_with_unct(prov['fit'])
                coded[code + 'e'] = _num(prov['exp'])
        coded['s' + rchar] = _number_with_unct(procs['fit'])
        coded['d' + rchar] = str(procs['obs'])
    return coded


_narrow_table = r"""
\begin{{tabular}}{{l|ccc}}
\hline\hline
\mct{{}} (GeV)  & $>$150 & $>$200 & $>$250 \\
 \hline
\multirow{{2}}{{*}}{{Top}}
         & {t15} & {t20} & {t25} \\
         & {t15e}  & {t20e} & {t25e} \\[4pt]
\multirow{{2}}{{*}}{{$Z$+jets}}
         & {z15} & {z20} & {z25} \\
         & {z15e}  & {z20e} & {z25e} \\[4pt]
\multirow{{2}}{{*}}{{$W$+jets}}
         & {w15} & {w20} & {w25} \\
         & {w15e}  & {w20e} & {w25e} \\[4pt]
Multijets & $0.3 \pm 0.3$ &  $0.2 \pm 0.2 $ &  $0.05 \pm 0.05$ \\
Others    & {o15} & {o20} & {o25} \\
\hline
{{\bf Total}} & {{\bf {s15} }} & {{\bf {s20} }} & {{\bf {s25} }} \\
\hline
{{\bf Data}}  & {{\bf {d15} }}& {{\bf {d20} }}& {{\bf {d25} }}\\
\hline\hline
\end{{tabular}}
"""

_wide_table = r"""
\begin{{tabular}}{{l||ccc||ccc}}
\hline\hline
 & \multicolumn{{3}}{{c||}}{{\bf Control Regions}} & \multicolumn{{3}}{{c}}{{{{\bf Signal Regions}}, \mct{{}} [GeV]}} \\
 & CRT & CRZ & CRW & $>$150 & $>$200 & $>$250 \\
 \hline
\multirow{{2}}{{*}}{{Top}}
         & {tt}   & {tz}   & {tw} &
         & {t15}  & {t20}  & {t25} \\
         & {tte}  & {tze}  & {twe} &
         & {t15e} & {t20e} & {t25e} \\
\multirow{{2}}{{*}}{{$Z$+jets}}
         & {zt}   & {zz}   & {zw} &
         & {z15}  & {z20}  & {z25} \\
         & {zte}  & {zze}  & {zwe} &
         & {z15e} & {z20e} & {z25e} \\
\multirow{{2}}{{*}}{{$W$+jets}}
         & {wt}   & {wz}   & {ww} &
         & {w15}  & {w20}  & {w25} \\
         & {wte}  & {wze}  & {wwe} &
         & {w15e} & {w20e} & {w25e} \\
Multijets &  --          &  --             &  --              &
          $0.3 \pm 0.3 $ &  $0.2 \pm 0.2 $ &  $0.05 \pm 0.05$ \\[4pt]
Others    & {ot}  & {oz}  & {ow}  &
          & {o15} & {o20} & {o25} \\
\hline
{{\bf Total}} & {st}           & {sz}           & {sw}           &
              & {{\bf {s15} }} & {{\bf {s20} }} & {{\bf {s25} }} \\
\hline
{{\bf Data}}  & {dt}           & {dz}           & {dw}           &
              & {{\bf {d15} }} & {{\bf {d20} }} & {{\bf {d25} }} \\
\hline\hline
\end{{tabular}}
"""


if __name__ == '__main__':
    run()

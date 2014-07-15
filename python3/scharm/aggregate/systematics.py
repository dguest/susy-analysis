"""
Functions to calculate sum of squares for the systematic variatons.
"""
from scharm.aggregate.aggschema import HistDict
from scharm.schema import nominal_syst, sys2_ext, wt2_ext
from scharm.schema import remove_minor_systematics

def _get_base_hist_tuples(syst_hists):
    base_tuples = set()
    for proc, var, region in syst_hists:
        bads = [var.endswith(wt2_ext), proc == 'data']
        if any(bads):
            continue
        base_tuples.add( (proc, var, region) )
    return base_tuples

def make_syst_aggregate(files, output_file):
    """expects 'files' to be keyed by systematic"""
    # set sig_prefix to None to pass through all signals
    syst_hists = HistDict(files[nominal_syst], sig_prefix=None)
    base_hist_names = _get_base_hist_tuples(syst_hists)
    variations = set(files) - set([nominal_syst])
    for var in remove_minor_systematics(variations):
        print('adding {}'.format(var))
        var_hists = HistDict(files[var])
        for bs_key in base_hist_names:
            if bs_key[0].startswith('scharm'):
                continue
            var = bs_key[1]
            base = syst_hists[bs_key]
            varied = var_hists[bs_key]

            # get the variation squared
            dif_sq = (varied - base)**2

            # save the varied hist
            sys_key = (bs_key[0], var + sys2_ext, bs_key[2])
            if not sys_key in syst_hists:
                syst_hists[sys_key] = dif_sq
            else:
                syst_hists[sys_key] += dif_sq

    syst_hists.write(output_file)


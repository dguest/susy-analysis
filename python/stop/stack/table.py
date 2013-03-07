from tempfile import TemporaryFile
import warnings, re

def _not_blinded(physics_cut_tup): 
    phys, cut = physics_cut_tup
    if phys != 'data': 
        return True
    ok_ends = ['cr', 'btag','lep']
    for end in ok_ends: 
        if cut.endswith(end): 
            return True
    return False
    

def get_physics_cut_dict(plots_dict, safe=True, notblind=_not_blinded): 
    """
    builds dictionary keyed by (physics type, cut name) containing the 
    counts of each (type, cut) pair
    """
    physcut_dict = {}
    used_var = {}
    for (phys, var, cut), hist in plots_dict.iteritems(): 
        physcut_tup = (phys, cut)
        # apply blinding
        if not notblind(physcut_tup):
            continue
        if physcut_tup not in physcut_dict: 
            physcut_dict[physcut_tup] = hist.array.sum()
            used_var[physcut_tup] = var
        elif safe: 
            thissum = hist.array.sum()
            diff = abs(physcut_dict[physcut_tup] - thissum)
            sums = abs(physcut_dict[physcut_tup] + thissum)
            if (diff or sums) and (diff and not sums or diff / sums > 0.001): 
                raise ValueError(
                    ("{} doesn't have consistent values" 
                     " ({} for {} vs {} for {})").format(
                        physcut_tup, 
                        physcut_dict[physcut_tup], used_var[physcut_tup], 
                        thissum, var))
    return physcut_dict

def make_latex_bg_table(physics_cut_dict, out_file=TemporaryFile()): 
    """
    returns a file-like object
    """
    from stop.style import type_dict

    phys_list, cut_list = zip(*physics_cut_dict.keys())
    phys_list = sorted(x for x in set(phys_list))
    cut_list = sorted(set(cut_list))
    texphys = []
    sigregex = re.compile('stop-[0-9]+-[0-9]+')
    for rawphys in phys_list: 
        if rawphys.startswith('stop'): 
            texphys.append(sigregex.search(rawphys).group(0))
        elif rawphys in type_dict: 
            texphys.append(type_dict[rawphys].tex)
        else: 
            texphys.append(rawphys)
    colstring = '|'.join(['c']*(len(texphys) +1))
    prereq = r'\begin{{tabular}}{{ {} }}'.format(colstring)
    out_file.write(prereq + '\n')
    headrow = r'& {} \\ \hline'.format(' & '.join(texphys))
    out_file.write(headrow + '\n')
    for cut in cut_list: 
        line = [cut.replace('_',' ')]
        for phys in phys_list: 
            tup = (phys,cut)
            if tup in physics_cut_dict: 
                value = physics_cut_dict[(phys,cut)]
                line.append('{:.1f}'.format(value))
            else: 
                line.append('XXX')
        textline = r'{} \\'.format(' & '.join(line))
        out_file.write(textline + '\n')

    out_file.write(r'\end{tabular}' + '\n')
    return out_file

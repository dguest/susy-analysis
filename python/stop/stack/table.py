from tempfile import TemporaryFile

def get_physics_cut_dict(plots_dict, safe=True): 
    """
    builds dictionary keyed by (physics type, cut name) containing the 
    counts of each (type, cut) pair
    """
    physcut_dict = {}
    used_var = {}
    for (phys, var, cut), hist in plots_dict.iteritems(): 
        physcut_tup = (phys, cut)
        if not cut.endswith('cr') and phys == 'data': 
            continue
        if physcut_tup not in physcut_dict: 
            physcut_dict[physcut_tup] = hist.array.sum()
            used_var[physcut_tup] = var
        elif safe: 
            if not physcut_dict[physcut_tup] == hist.array.sum(): 
                raise ValueError(
                    ("{} doesn't have consistent values" 
                     " ({} for {} vs {} for {})").format(
                        physcut_tup, 
                        physcut_dict[physcut_tup], used_var[physcut_tup], 
                        hist.array.sum(), var))
    return physcut_dict

def make_latex_bg_table(physics_cut_dict, out_file=TemporaryFile()): 
    """
    returns a file-like object
    """
    from stop.style import type_dict

    phys_list, cut_list = zip(physics_cut_dict.keys())
    texphys = []
    for rawphys in phys_list: 
        if rawphys.startswith('stop'): 
            continue
        if rawphys in type_dict: 
            texphys.append(type_dict[rawphys].tex)
        else: 
            texphys.append(rawphys)
    colstring = '|'.join(['c']*(len(texphys) +1))
    prereq = r'\begin{{tabular}}{{ {} }}'.format(colstring)
    out_file.write(prereq + '\n')
    headrow = r'&& {} \hline'.format('&&'.join(texphys))
    out_file.write(headrow + '\n')
    for cut in cut_list: 
        line = [cut.replace('_',' ')]
        for phys in rawphys: 
            value = physics_cut_dict[(phys,cut)]
            line.append('{:.1f}'.format(value))
        textline = '&&'.join(line)
        out_file.write(textline + '\n')

    return out_file

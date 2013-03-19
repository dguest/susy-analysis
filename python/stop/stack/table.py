from tempfile import TemporaryFile as Temp
import warnings, re
from stop.stack.coordinator import Region 
from stop.style import texify_sr

# not used any more
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

def yamlize(physics_cut_dict): 
    """
    converts the physics_cut_dict into a nested dict structure. 
    Schema is as follows: 
        - systematic
        - physics type
        - region 
        - cut count
    """
    if not isinstance(physics_cut_dict, dict): 
        return float(physics_cut_dict)
    ymlout = {}
    for key, entry in physics_cut_dict.iteritems(): 
        if isinstance(key, tuple): 
            f, s = [str(i) for i in key]
            if not f in ymlout: 
                ymlout[f] = {s:yamlize(entry)}
            else: 
                ymlout[f][s] = yamlize(entry)
        else: 
            ymlout[str(key)] = yamlize(entry)

    return ymlout

def unyamlize(yaml_physics_dict): 
    """
    starts with one systematic entry, builds a (physics, cut): count dict
    """
    out = {}
    for phys, cutdict in yaml_physics_dict.iteritems(): 
        for cut, number in cutdict.iteritems(): 
            tup = (phys, cut)
            out[tup] = number

    return out

def make_latex_bg_table(physics_cut_dict, out_file=Temp(), title=''): 
    """
    returns a file-like object
    """
    from stop.style import type_dict

    phys_list, cut_list = zip(*physics_cut_dict.keys())
    phys_list = sorted(x for x in set(phys_list))
    def cr_sort(key): 
        splkey = key.split('_')
        if len(splkey) == 1: 
            return '1' + key
        return ''.join(splkey[::-1])
    cut_list = sorted(set(cut_list), key=cr_sort)
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
    headrow = r' {} & {} \\ \hline'.format(title,' & '.join(texphys))
    out_file.write(headrow + '\n')
    for cut in cut_list: 
        line = [texify_sr(cut)]
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

def _get_fields(region): 
    region_dict =  {
        'jet1pt':region.kinematics['leading_jet_gev'], 
        'met': region.kinematics['met_gev'], 
        }
    for num, tag in enumerate(region.btag_config): 
        region_dict['Jet {} Tag'.format(num + 1)] = tag
    return region_dict


class LatexCutsConfig(object): 

    fields = [
        r'name', 'met', 'jet1pt', 
        r'Jet 1 Tag', r'Jet 2 Tag', r'Jet 3 Tag', r'Jet 4 Tag']
    field_aliases = { 
        'met':r'$E_{\mathrm{T}}^{\mathrm{miss}}\,[\text{GeV}]$',
        'jet1pt':r'Leading Jet $p_{\mathrm{T}}\,[\text{GeV}]$', 
        }
    replace_values = { 
        'NOTAG':'', 
        }

    def __init__(self): 
        pass

    def _begin(self): 
        fields = self.fields
        colstring = '|'.join(['c']*len(self.fields))
        prereq = r'\begin{{tabular}}{{ {} }}'.format(colstring)
        return prereq

    def _fields(self): 
        fields = self.fields
        field_aliases = self.field_aliases
        headers = [
            field_aliases[x] if x in field_aliases else x for x in fields]
        head_line = r' & '.join(headers) + r'\\ \hline'
        return head_line + '\n'

    def _end(self): 
        return r'\end{tabular}' + '\n'

    def latex_config_file(self, cuts_dict): 
        signal_regions = {}
        control_regions = {}
        regions = cuts_dict['regions']
        for name, region_dict in regions.iteritems(): 
            region = Region(region_dict)
            if region.type == 'signal': 
                signal_regions[name] = region 
            elif region.type == 'control': 
                control_regions[name] = region
    
        sort_sr = sorted(signal_regions.iteritems())
        full_file = Temp()
        full_file.write(self._begin())

        n_fields = len(self.fields)

        sr_line = (
            r'\multicolumn{{{}}}{{c}}{{Signal Regions}}\\ \hline'.format(
            n_fields) )
        full_file.write(sr_line + '\n')
        full_file.write(self._fields())
        for line in self.latex_cuts_config(sort_sr): 
            full_file.write(line)
        full_file.write(r'\hline' + '\n')
            
        cr_line = (
            r'\multicolumn{{ {} }}{{c}}{{Control Regions}}\\ \hline'.format(
            n_fields) )
        full_file.write(cr_line + '\n')
        full_file.write(self._fields())
        def cr_key(cr_tup): 
            name, reg = cr_tup
            splname = name.split('_')
            return ''.join(splname[::-1])
        sort_cr = sorted(control_regions.iteritems(), key=cr_key)
        for line in self.latex_cuts_config(sort_cr): 
            full_file.write(line)
            
        full_file.write(self._end())
        full_file.seek(0)
        return full_file
    
    def latex_cuts_config(self, signal_regions):
        fields = self.fields
        field_aliases = self.field_aliases
        int_file = Temp()
        for sr_name, sr in signal_regions: 
            field_values = _get_fields(sr)
            tex_name = texify_sr(sr_name)
            sr_fields = [tex_name]
            for x in fields[1:]: 
                try: 
                    field_value = str(field_values[x])
                    if field_value in self.replace_values: 
                        field_value = self.replace_values[field_value]
                except KeyError: 
                    field_value = ''
                sr_fields.append(field_value)
    
            line = r' & '.join(sr_fields) + r'\\'
            int_file.write(line + '\n')
    
        int_file.seek(0)
        return int_file

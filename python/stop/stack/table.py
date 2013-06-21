from tempfile import TemporaryFile as Temp
import warnings, re
from stop.stack.regions import Region 
from stop.style import texify_sr
from math import log
from collections import Counter

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


def get_physics_cut_dict(plots_dict, safe=True): 
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

def sort_physics(phys_list): 
    bgs = []
    stops = []
    other = []
    for phys in phys_list: 
        if 'stop' in phys: 
            stops.append(phys)
        elif phys == 'data': 
            other.append(phys)
        else: 
            bgs.append(phys)
    return bgs + sorted(stops) + other

class RegionCountsFormatter(object): 
    def __init__(self, cut): 
        self.line = [cut]
        self.total_bg = 0.0
        self.total_stats = 0
        self.max_counts = 0.0
        self.max_idx = None
    def _add_value(self, value, error): 
        if not error: 
            prec = 0
        else: 
            prec = max(-int(log(error)) + 1, 0)
        self.line.append('{n:.{p}f}({e:.{p}f})'.format(
                n=value, p=prec, e=error))
        
    def add_bg_value(self, value): 
        if not value: 
            self.line.append('XXX')
            return 

        normed = value['normed']
        stats = value['stats']
        if not stats: 
            stat_err = 0.0
        else: 
            stat_err = stats**0.5 / stats * normed
        self._add_value(normed, stat_err)
        self.total_bg += normed
        self.total_stats += stats
        if value > self.max_counts: 
            self.max_counts = value
            self.max_idx = len(self.line) - 1
    def add_other(self, value): 
        if not value: 
            self.line.append('XXX')
            return 

        try: 
            normed = value['normed']
            stats = value['stats']
            try: 
                stat_err = stats**0.5 / stats * normed
            except ZeroDivisionError: 
                stat_err = 0
            self._add_value(normed, stat_err)
        except TypeError: 
            try: 
                normed = int(value)
            except ValueError: 
                normed = value
            self.line.append(str(normed))

    def get_line(self, redmax=True, total=True): 
        if self.max_idx is not None and redmax: 
            self.line[self.max_idx] = r'\textcolor{{red}}{{{}}}'.format(
                self.line[self.max_idx])
        if total: 
            self.line.append('{:.1f}'.format(self.total_bg))
        return r'{} \\'.format(' & '.join(self.line))


def make_latex_bg_table(physics_cut_dict, out_file=Temp(), title=''): 
    """
    Makes table with columns named by mc type, rows named by region. 

    returns a file-like object. 
    """
    from stop.style import type_dict

    phys_list, cut_list = zip(*physics_cut_dict.keys())
    phys_list = sort_physics(set(phys_list))
    def cr_sort(key): 
        splkey = key.split('_')
        if len(splkey) == 1: 
            return '-' + key
        return ''.join(splkey[::-1])
    cut_list = sorted(set(cut_list), key=cr_sort)
    sigregex = re.compile('stop-[0-9]+-[0-9]+')
    signals = [s for s in phys_list if sigregex.search(s)]
    for signal in signals: 
        phys_list.append(phys_list.pop(phys_list.index(signal)))
    datname = [x for x in phys_list if x.lower() == 'data']
    if datname: 
        phys_list.append(phys_list.pop(phys_list.index(datname[0])))
    texphys = []
    for rawphys in phys_list: 
        if rawphys.startswith('stop'): 
            texphys.append(rawphys)
        elif rawphys in type_dict: 
            texphys.append(type_dict[rawphys].tex)
        else: 
            texphys.append(rawphys)
    texphys.append('total BG')
    colstring = '|'.join(['c']*(len(texphys) +1))
    prereq = r'\begin{{tabular}}{{ {} }}'.format(colstring)
    out_file.write(prereq + '\n')
    headrow = r' {} & {} \\ \hline'.format(title,' & '.join(texphys))
    out_file.write(headrow + '\n')
    for cut in cut_list:
        line_formatter = RegionCountsFormatter(texify_sr(cut))
        for phys in phys_list: 
            tup = (phys,cut)
            if tup in physics_cut_dict: 
                value = physics_cut_dict[phys,cut]
                if phys.lower() == 'data' or phys.startswith('stop'): 
                    line_formatter.add_other(value)
                else: 
                    line_formatter.add_bg_value(value)
            else: 
                line_formatter.add_other(None)
        textline = line_formatter.get_line()
        out_file.write(textline + '\n')

    out_file.write(r'\end{tabular}' + '\n')
    return out_file

def make_marktable(physics_cut_dict, out_file, title=''): 
    """
    Makes table with columns named by region, rows named by physics type. 

    returns a file-like object. 
    """
    from stop.style import type_dict

    phys_list, reg_list = zip(*physics_cut_dict.keys())
    phys_list = sort_physics(set(phys_list))
    def cr_sort(key): 
        splkey = key.split('_')
        if len(splkey) == 1: 
            return '-' + key
        return ''.join(splkey[::-1])
    reg_list = sorted(set(reg_list), key=cr_sort)
    sigregex = re.compile('stop-[0-9]+-[0-9]+')
    signals = [s for s in phys_list if sigregex.search(s)]
    for signal in signals: 
        phys_list.append(phys_list.pop(phys_list.index(signal)))
    datname = [x for x in phys_list if x.lower() == 'data']
    if datname: 
        phys_list.append(phys_list.pop(phys_list.index(datname[0])))
    texphys = []
    for rawphys in phys_list: 
        if rawphys.startswith('stop'): 
            texphys.append(rawphys)
        elif rawphys in type_dict: 
            texphys.append(type_dict[rawphys].tex)
        else: 
            texphys.append(rawphys)
    texphys.append('total BG')
    colstring = '|'.join(['c']*(len(reg_list) +1))
    prereq = r'\begin{{tabular}}{{ {} }}'.format(colstring)
    out_file.write(prereq + '\n')
    texreg = [texify_sr(x) for x in reg_list]
    headrow = r' {} & {} \\ \hline'.format(title,' & '.join(texreg))
    bg_counter = Counter()
    out_file.write(headrow + '\n')
    for phys in phys_list:
        line_formatter = RegionCountsFormatter(phys)
        for cut in reg_list: 
            tup = (phys,cut)
            if tup in physics_cut_dict: 
                value = physics_cut_dict[phys,cut]
                if phys.lower() == 'data' or phys.startswith('stop'): 
                    line_formatter.add_other(value)
                else: 
                    line_formatter.add_bg_value(value)
                    bg_counter[cut] += value['normed']
            else: 
                line_formatter.add_other(None)
        textline = line_formatter.get_line(redmax=False, total=False)
        out_file.write(textline + '\n')

    bg_total = RegionCountsFormatter('Total BG')
    for cut in reg_list: 
        bg_total.add_other(bg_counter[cut])
    out_file.write(bg_total.get_line(redmax=False,total=False) + '\n')

    ratio_line = RegionCountsFormatter('Data / MC')
    for cut in reg_list: 
        try: 
            ratio = float(physics_cut_dict['data', cut]) / bg_counter[cut] 
            ratio_line.add_other('{:.2f}'.format(ratio))
        except KeyError: 
            ratio_line.add_other(None)
    out_file.write(ratio_line.get_line(redmax=False, total=False) + '\n')
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

import re
_inf = float('inf')
# __________________________________________________________________________
# sample / stack labeling stuff
physics_labels = [
    (r'diboson',                'diboson','pink'), 
    (r'QCD',                    'QCD','c'), 
    (r'single $t$',             'singleTop','teal'),
    (r'$t\bar{t}$',             'ttbar','b'),
    (r'$t\bar{t} + V$',         'ttbarV','brown'),
    (r'$W \to l \nu$ + b',   'WjetsB','red'), 
    (r'$W \to l \nu$ + c',   'WjetsC','yellow'), 
    (r'$W \to l \nu$ + u',   'WjetsL','purple'), 
    (r'$Z \to \nu \nu$ + b', 'ZjetsB','orange'), 
    (r'$Z \to \nu \nu$ + c', 'ZjetsC','green'), 
    (r'$Z \to \nu \nu$ + u', 'ZjetsL','m'), 
    # (r'$W$ + jets',             'Wjets','green'), 
    (r'Data',                   'data'   , 'black'), 

    ('charm','charm','g'), 
    ('bottom','bottom','red'), 
    ('light','light','blue'), 
    ('tau','tau','pink'), 
    ('other','other','orange'), 
    
    ]

def texify_sr(sr_name): 
    sr_name = sr_name.replace('-',r'\,')
    if '_' in sr_name: 
        prenanme, throw_away, name = sr_name.partition('_')
        tex_name = r'$\mathrm{{{}}}_{{\mathrm{{{}}} }}$'.format(
            prenanme, name.replace('_',' '))
    else: 
        tex_name = r'$\mathrm{{ {} }}$'.format(sr_name)
    return tex_name

def cr_sort(key): 
    splkey = key.split('_')
    if len(splkey) == 1: 
        return '1' + key
    return ''.join(splkey[::-1])

class StackStyle(object): 
    def __init__(self, tex_name, label, color): 
        self.tex = tex_name
        self.lable = label
        self.color = color

type_dict = {l: StackStyle(t,l,c) for t,l,c in physics_labels}

# __________________________________________________________________________
# axis labeling stuff

def get_variable_style(ax_name, units): 
    """
    Top level axis style function, should be all you need to call from 
    the plotting scripts. Tries to match variable exactly, then tries
    regular expressions, then just returns the input.
    """
    if ax_name in _ax_labels: 
        return _ax_labels[ax_name]
    
    for matcher in _fuzzy_matches: 
        match = matcher(ax_name)
        if match is not None: 
            return match

    return VariableStyle(ax_name.replace('_',' '), units)

class VariableStyle(object): 
    def __init__(self, tex_name, units='', plot_range=None): 
        self.tex_name = tex_name
        self.units = str(units)
        self.plot_range = plot_range

def _make_jet_matcher(match_token, tex_str, units=''): 
    """
    Build a function to try to match jet variable names.
    """
    regex = re.compile('j([0-9])_{}'.format(match_token))
    def matcher(in_str): 
        matches = regex.search(in_str)
        if not matches: 
            return None
        jet_n = int(matches.group(1)) + 1
        tex = r'Jet {n} {}'.format(tex_str, n=jet_n)
        return VariableStyle(tex, units)
    return matcher

# things in here return None if they don't match, otherwise return the new 
# variable name
_fuzzy_matches = [
    _make_jet_matcher('eta', r'$\eta$'),
    _make_jet_matcher('pt', r'$p_{\mathrm{T}}$', 'GeV'),
    _make_jet_matcher('antib', r'JFC $\log (c / b)$'),
    _make_jet_matcher('antiu', r'JFC $\log (c / u)$')
    ]

_shorthand = dict(
    met=r'E_{\mathrm{T}}^{\mathrm{miss}}', 
    met_eff=r'E_{\mathrm{T}, \mathrm{eff}}^{\mathrm{miss}}', 
    pt=r'p_{\mathrm{T}}',
    meff=r'\sum_{i = \{1,2\} } p_{\mathrm{T}}^i'
)
    

_ax_labels = { 
    'met' : VariableStyle(r'$E_{\mathrm{T}}^{\mathrm{miss}}$', 'GeV'), 
    'met_eff' : VariableStyle(
        r'${met_eff} \equiv {met} / \left[ {meff} + {met} \right]$'.format(
            **_shorthand)), 
    'mttop': VariableStyle(r'$m_{\mathrm{T}}^t$', 'GeV'), 
    'htx': VariableStyle(r'$H_{\mathrm{T}3}$'), 
    'jetmet_dphi': VariableStyle(
        r'$\min_{i = \{1,2,3\}}\left(\Delta \phi (j_{i}, '
        r'E_{\mathrm{T}}^{\mathrm{miss}})\right)$'),
    'leading_lepton_pt': VariableStyle(
        r'Leading Lepton $p_{\mathrm{T}}$', 'GeV'),
    'mass_t': VariableStyle('$m_{\mathrm{T}}$', 'GeV', (0,200)),
    'mass_cc': VariableStyle('$m_{cc}$', 'GeV'), 
    'mass_ll': VariableStyle('$m_{\ell \ell}$', 'GeV', (0,200)), 
    'mass_ct': VariableStyle('$m_{\mathrm{CT}}$', 'GeV'),
    }

# _________________________________________________________________________
# variable range cropping 
# NOTE: this is "plot level" units, the cropping is done _after_ 
#       units are converted
crop_vars = { 
    'mass_ll': (40, 140),
    'j1_pt': (0, 500),
    'j2_pt': (0, 200),
}

# ________________________________________________________________________
# rebinning
rebinning = {
    'preselection':100,
}

# _________________________________________________________________________
# misc things

def event_label(lumi): 
    return r'Events / {:.1f} fb$^{{-1}}$'.format(lumi)

hdict = dict(x=0.98, ha='right', fontsize=18)
vdict = dict(y=0.98, ha='right', fontsize=18)

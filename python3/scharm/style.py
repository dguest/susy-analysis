import re
_inf = float('inf')
# __________________________________________________________________________
# sample / stack labeling stuff

class StackStyle:
    def __init__(self, tex_name, label, color):
        self.tex = tex_name
        self.lable = label
        self.color = color

def get_type_dict(theme='dan'):
    if theme in _theme_names:
        theme = _theme_names[theme]
    return {l: StackStyle(t,l,c) for t,l,c in _get_labels(theme)}

def get_selection_color(theme):
    """for selection plots"""
    if theme in _theme_names:
        theme = _theme_names[theme]
    return theme.get('cut_line','k'), theme.get('cut_fill',(0,0,0,0.2))

def get_signal_colors(theme):
    if theme in _theme_names:
        theme = _theme_names[theme]
    return theme.get('signal_list', 'kcmy')

def get_signal_dashes(theme):
    if theme in _theme_names:
        theme = _theme_names[theme]
    return theme.get('signal_dashes', [])

# define the themes here
_dan_detail = {
    'diboson':'pink',
    'QCD':'c',
    'singleTop': 'teal',
    'ttbar':'b',
    'ttbarV':'brown',
    'WjetsB':'red',
    'WjetsC':'yellow',
    'WjetsL':'purple',
    'ZjetsB':'orange',
    'ZjetsC':'green',
    'ZjetsL':'m',
    }

_dan_theme = _dan_detail.copy()
_dan_theme.update( {
        'other':'orange',
        'Wjets':'yellow',
        'Zjets':'green',
        # 'top':'blue',
        'top': (0.4,0.4,1.0),
        # 'top':(0x33/255,0x33/255,0xCC/255), # sbottom Wjets
        'signal_list': ['purple', 'cyan'],
        })

_light_theme = _dan_detail.copy()
_light_theme.update( {
        'other':'orange',
        'Wjets':'yellow',
        'Zjets':'lightgreen',
        # 'top':'blue',
        # 'top': 'lightblue',
        'top': 'skyblue',
        # 'top':(0x33/255,0x33/255,0xCC/255), # sbottom Wjets
        'signal_list': ['k', 'k'],
        'signal_dashes': [[15, 3],[2,2]*2],
        })

_brimstone = _dan_detail.copy()
_brimstone.update( {
        'other':'yellow',
        'Wjets':'FireBrick',
        'Zjets':'Maroon',
        'top':'OrangeRed',
        'cut_line':'r',
        'cut_fill': (0.9, 0, 0, 0.2),
        'signal_list': ['black','gold','orange','red'],
        })
_brony = _dan_detail.copy()
_brony.update( {
        'other':'LightBlue',
        'Wjets':'DeepPink',
        'Zjets':'MediumSpringGreen',
        'top':'Orchid',
        'cut_line': (1,0,1),
        'cut_fill': (1,0,1,0.2),
        'signal_list': ['DarkOrchid', 'DarkCyan', 'HotPink'],
        })

_sbot_theme = {
    'diboson':'pink',
    'QCD':'c',
    'singleTop': 'teal',
    'ttbar':'b',
    'ttbarV':'brown',
    'WjetsB':'red',
    'WjetsC':'yellow',
    'WjetsL':'purple',
    'ZjetsB':'orange',
    'ZjetsC':'green',
    'ZjetsL':'m',

    'other':(0.800,0.400,0.000),
    'Wjets':(0.200,0.200,0.800),
    'Zjets':(0.000,0.000,0.400),
    'top'  :(0.000,1.000,0.000),
    'signal_list': ['orange','red'],
    }

_theme_names = {
    'dan':_dan_theme, 'sbot': _sbot_theme,
    'brimstone': _brimstone, 'brony':_brony,
    'light':_light_theme}

def _get_labels(theme):
    def get_theme(texname, name):
        return texname, name, theme[name]
    return [
        get_theme(r'Diboson',           'diboson'),
        get_theme(r'QCD',               'QCD'),
        get_theme(r'Single $t$',        'singleTop'),
        get_theme(r'$t\bar{t}$',        'ttbar'),
        get_theme(r'$t\bar{t} + V$',    'ttbarV'),
        get_theme(r'$W \to l \nu$ + b', 'WjetsB'),
        get_theme(r'$W \to l \nu$ + c', 'WjetsC'),
        get_theme(r'$W \to l \nu$ + u', 'WjetsL'),
        get_theme(r'$Z$ + b',           'ZjetsB'),
        get_theme(r'$Z$ + c',           'ZjetsC'),
        get_theme(r'$Z$ + u',           'ZjetsL'),

        (r'Data',               'data'   , 'black'),

        ('charm','charm','g'),
        ('bottom','bottom','red'),
        ('light','light','blue'),
        ('tau','tau','pink'),
        ('other','other','orange'),

        # colors from the sbottom analysis
        get_theme(r'Other'     , 'other'),
        get_theme(r'$W$ + jets', 'Wjets'),
        get_theme(r'$Z$ + jets', 'Zjets'),
        get_theme(r'Top'       , 'top'  ),
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

_shorthand = dict(
    met=r'E_{\rm T}^{\rm miss}',
    met_eff=r'E_{\rm T, eff}^{\rm miss}',
    pt=r'p_{\mathrm{T}}',
    meff=r'\sum_{i = \{1,2\} } p_{\rm T}^i'
)
def _sh(string):
    """replace shorthand in some string"""
    return string.format(**_shorthand)

# things in here return None if they don't match, otherwise return the new
# variable name
_fuzzy_matches = [
    _make_jet_matcher('eta', r'$\eta$'),
    _make_jet_matcher('pt', r'$p_{\rm T}$', 'GeV'),
    _make_jet_matcher('antib', r'JFC $\log (c / b)$'),
    _make_jet_matcher('antiu', r'JFC $\log (c / u)$'),
    _make_jet_matcher('flavor_truth_label', r'flavor truth label'),
    _make_jet_matcher('met_dphi', _sh(r'$\Delta \phi({met},j)$')),
    ]


_ax_labels = {
    'met' : VariableStyle(_sh(r'${met}$'), 'GeV'),
    'met_eff' : VariableStyle(
        _sh(r'${met_eff} \equiv {met} / \left[ {meff} + {met} \right]$')),
    'mttop': VariableStyle(r'$m_{\rm T}^t$', 'GeV'),
    'htx': VariableStyle(r'$H_{\rm T3}$'),
    'jetmet_dphi': VariableStyle(
        r'$\min_{i = \{1,2,3\}}\Delta \phi (j_{i}, '
        r'E_{\rm T}^{\rm miss})$'),

    'leading_lepton_pt': VariableStyle(
        r'Leading Lepton $p_{\mathrm{T}}$', 'GeV'),
    'second_lepton_pt': VariableStyle(
        r'Second Lepton $p_{\mathrm{T}}$', 'GeV'),
    'leading_lepton_met_dphi': VariableStyle(
        _sh(r'Leading Lepton $\Delta \phi({met},\ell)$')),
    'second_lepton_met_dphi': VariableStyle(
        _sh(r'Second Lepton $\Delta \phi({met},\ell)$')),

    'mass_t': VariableStyle(r'$m_{\rm T}$', 'GeV', (0,200)),
    'mass_cc': VariableStyle('$m_{cc}$', 'GeV'),
    'mass_ll': VariableStyle('$m_{\ell \ell}$', 'GeV', (0,200)),
    'mass_ct': VariableStyle(r'$m_{\rm CT}$', 'GeV'),
    'mass_ct_uncorr': VariableStyle(r'$m_{\rm CT}^{\rm raw}$', 'GeV'),
    'dphi_any': VariableStyle(
        r'$\min_{v \in \{j_1, j_2, j_3, \ell_1, \ell_2 \} }\Delta \phi (v, '
        r'E_{\rm T}^{\rm miss})$'),
    'dphi_cc': VariableStyle('$\Delta \phi (j_1, j_2)$'),
    'dphi_ll': VariableStyle('$\Delta \phi (\ell_1, \ell_2)$'),
    'dr_cc': VariableStyle('$\Delta R (j_1, j_2)$'),
    }

# _________________________________________________________________________
# variable range cropping
# NOTE: this is "plot level" units, the cropping is done _after_
#       units are converted
crop_vars = {
    'met': (0, 500),
    'mass_ct': (0, 400),
    'mass_ll': (40, 140),
    'j0_pt': (0, 500),
    'j1_pt': (0, 300),
    'j2_pt': (0, 200),
    'n_signal_jets': (1.5, 10.5),
    'mass_t': (0, 200),
    'leading_lepton_pt': (0, 350),
    'second_lepton_pt': (0, 160),
    }
# if defined, use the special 'log' values for log plots 
# (if not listed below)
log_crop_vars = {
    'mass_ct': (0, 500),
    }
# cropping for specific regions (given as (region, variable))
crop_region_vars = {
    ('cr_t', 'met'): (0, 400),
    ('cr_w', 'met'): (0, 400),
    ('cr_z', 'met'): (0, 400),
    ('signal_mct150', 'met'): (100, 500),
    ('signal_mct200', 'met'): (100, 500),
    ('signal_mct250', 'met'): (100, 500),
    }

# ________________________________________________________________________
# misc top level stack plotting tweaks

def customize_stack(stack, var_name, region, is_paper):
    is_sr = region.startswith('signal')
    if var_name.endswith('flavor_truth_label'):
        stack.ax.set_xticks([0, 1, 2, 3])
        stack.ax.set_xticklabels(['light', 'charm', 'bottom', 'tau'])
        stack.ax.set_ylabel('Jets')
    if var_name == 'mass_ct' and is_sr:
        stack.add_cut_arrow(150)
        stack.add_cut_arrow(200)
        stack.add_cut_arrow(250)
        stack.region_name = None

    # hacks for Al
    if (var_name, region) == ('mass_cc','signal_mct150'):
        stack.lumi_info_position = (0.52, 0.88)
        stack.extra_yrange = 0.75

def predraw_customize(stack, var_name, region, is_paper):
    if not is_paper:
        return

    is_sr = region.startswith('signal')
    # hack for Troels
    if var_name == 'met' and is_sr:
        stack.ratio_max = 3.0
# ________________________________________________________________________
# rebinning
rebinning = {
    'preselection':100,
    # 'signal_mct150': 15,
}

# _________________________________________________________________________
# misc things

def event_label(lumi):
    return r'Events / {:.1f} fb$^{{-1}}$'.format(lumi)

hdict = dict(x=0.98, ha='right', fontsize=18)
vdict = dict(y=0.98, ha='right', fontsize=18)

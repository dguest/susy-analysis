physics_labels = [
    (r'$Z \to \nu \nu$ + jets', 'Znunujets','orange'), 
    (r'$Z \to ll$ + jets',      'Zll','c'), 
    (r'$W \to l \nu$ + jets',   'Wlnu','red'), 
    (r'$W$ + jets',             'Wjets','green'), 
    (r'$t\bar{t}$',             'ttbar','b'),
    (r'diboson',                'diboson','pink'), 
    (r'Data',                   'data'   , 'black'), 

    ('charm','charm','g'), 
    ('bottom','bottom','red'), 
    ('light','light','blue'), 
    ('tau','tau','pink'), 
    ('other','other','orange'), 
    
    ]


class StackStyle(object): 
    def __init__(self, tex_name, label, color): 
        self.tex = tex_name
        self.lable = label
        self.color = color

type_dict = {l: StackStyle(t,l,c) for t,l,c in physics_labels}


class VariableStyle(object): 
    def __init__(self, tex_name, units=''): 
        self.tex_name = tex_name
        self.units = units
    @property
    def axis_label(self): 
        if self.units:
            return '{} [{}]'.format(self.tex_name, self.units)
        else: 
            return self.tex_name

    def rescale(self, cord): 
        if self.units == 'GeV': 
            return cord / 1e3
        else:
            return cord

ax_labels = { 
    'met' : VariableStyle(r'$E_{\mathrm{T}}^{\mathrm{miss}}$', 'GeV'), 
    'pt': VariableStyle(r'$p_{\mathrm{T}}$', 'GeV'), 
    'mttop': VariableStyle(r'$m_{\mathrm{T}}^t$', 'GeV'), 
    'htx': VariableStyle(r'$H_{\mathrm{T}3}$'), 
    'metDphi': VariableStyle(
        r'$\Delta \phi (j, E_{\mathrm{T}}^{\mathrm{miss}})$'), 
    'minDphi': VariableStyle(
        r'$\min_{i = \{1,2,3\}}\left(\Delta \phi (j_{i}, E_{\mathrm{T}}^{\mathrm{miss}})\right)$')
    }

def event_label(lumi): 
    return r'Events / {:.1f} fb$^{{-1}}$'.format(lumi)

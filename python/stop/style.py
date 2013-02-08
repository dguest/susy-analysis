physics_labels = [
    (r'$Z \to \nu \nu$ + jets', 'Znunujets','orange'), 
    (r'$Z \to ll$ + jets',      'Zll','c'), 
    (r'$W \to l \nu$ + jets',   'Wlnu','red'), 
    (r'$W$ + jets',             'Wjets','green'), 
    (r'$t\bar{t}$',             'ttbar','b'),
    (r'diboson',                'diboson','pink'), 

    ('charm','charm','g'), 
    ('bottom','bottom','red'), 
    ('light','light','blue'), 
    ('tau','tau','pink'), 
    ('other','other','orange'), 
    
    ]

class Style(object): 
    def __init__(self, tex_name, label, color): 
        self.tex = tex_name
        self.lable = label
        self.color = color

type_dict = {l: Style(t,l,c) for t,l,c in physics_labels}

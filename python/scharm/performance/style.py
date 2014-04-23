import re
def get_jet_eff_color(sample):
    custom_colors = [
        ('Sherpa.*LeptHad', 'red'),
        ('Sherpa.*Wjets', 'blue'),
        ('Sherpa.*Zjets', 'black'),
        ('McAt', 'purple'),
        ('Powheg', 'green'),
        ]
    for sample_re, color in custom_colors:
        if re.compile(sample_re).search(sample):
            return color

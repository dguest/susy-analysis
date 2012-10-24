from jetnet.cxxprofile import pro2d, profile_fast
import bits
import os

def cum_cuts(bits=bits.bits, other_bits=bits.other_bits): 
    base_mask = 0
    for (name, value), other_name in zip(bits,other_bits): 
        base_mask |= value
        yield other_name, base_mask


def build_profile(root_file, put_where='cache'): 
    out_file2_name = '{}_pro2{}'.format(*os.path.splitext(root_file))
    out_file2_name = os.path.join(put_where,os.path.basename(out_file2_name))

    if not os.path.isdir(put_where): 
        os.mkdir(put_where)
        
    met = ('met', 100, 0, 1000000)
    n_jet_range = 15
    n_susy_jets = ('n_susy_jets', n_jet_range, -0.5, -0.5 + n_jet_range)
    n_susy_jets = ('n_good_jets', n_jet_range, -0.5, -0.5 + n_jet_range)
    jetmet_dphi_min = ('min_jetmet_dphi', 100, -0.1, 3.3)
    
    def cnn_cb(jets):
        return ('{}_jet_cnn_log_cb'.format(jets), 100, -8, 4)
    def cnn_cu(jets):
        return ('{}_jet_cnn_log_cu'.format(jets), 100, -8, 8)
        
    jets = ['jet1','jet2','jet3']

    jet_pt = [('{}_pt'.format(j), 100, 0, 1000000) for j in jets]
    jet_eta = [('{}_eta'.format(j), 100, -3, 3) for j in jets]

    all_vars = jet_pt + jet_eta + [n_susy_jets, jetmet_dphi_min, met]

    all_cats = []
    masks = []
    for name, mask in cum_cuts(): 
        parsable_name = 'cut_{}'.format(name)
        the_dict = dict(name=parsable_name, leaf_name='pass_bits', 
                        accept_mask=mask)
        masks.append(the_dict)
        all_cats.append(name)

    two_plots = []
    for v in all_vars: 
        two_plots.append( (v, met))

    if not os.path.isfile(out_file2_name): 
        output_file, hists = pro2d(
            in_file=root_file, out_file=out_file2_name, tree='evt_tree',
            plots=two_plots, 
            masks=masks, 
            show_progress=True)

    return out_file2_name

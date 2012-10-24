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
        
    met = ('met', 2, 0, 1000000)
    jet1_pt = ('jet1_pt', 100, 0, 1000000)
    jet1_eta = ('jet1_eta', 100, -3, 3)
    n_jet_range = 15
    n_susy_jets = ('n_susy_jets', n_jet_range, -0.5, -0.5 + n_jet_range)
    n_susy_jets = ('n_good_jets', n_jet_range, -0.5, -0.5 + n_jet_range)
    jetmet_dphi_min = ('min_jetmet_dphi', 100, -0.1, 3.3)
    
    def cnn_cb(jets):
        return ('{}_jet_cnn_log_cb'.format(jets), 100, -8, 4)
    def cnn_cu(jets):
        return ('{}_jet_cnn_log_cu'.format(jets), 100, -8, 8)
        
    jets = ['jet1','jet2']

    all_vars = [jet1_pt, jet1_eta, n_susy_jets, jetmet_dphi_min]

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

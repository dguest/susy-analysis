from stop.hists import HistNd
from h5py import File

class RegionExtractor(object): 
    """
    Class is built from superregion defs, extracts region counts. 
    """
    def __init__(self, superregions): 
        self.superregions = superregions
        
    def extract_counts(self, h5_file_name): 
        with File(h5_file_name) as h5: 
            for supername, superregion in self.superregions.iteritems(): 
                normed = HistNd(h5[supername]['kinematics'])
                normed_axes = {a.name:a for a in normed.axes}
                for name, region in superregion['subregions'].iteritems(): 
                    kin = region['kinematics']
                    met_cut = kin['met_gev']*1e3
                    lead_cut = kin['leading_jet_gev']*1e3
                    print name
                    # print met_cut, normed_axes['leadingJetPt'].extent
                    print lead_cut, normed_axes['leadingJetPt'].extent

                    lead_ax = normed_axes['leadingJetPt']
                    lead_slice, lead_ext = lead_ax.get_slice(
                        lead_cut*0.9999)
                    print lead_ext
                    

                                

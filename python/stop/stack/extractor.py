from stop.hists import HistNd
from h5py import File
from os.path import isfile

class RegionExtractor(object): 
    """
    Class is built from superregion defs, extracts region counts. 
    """
    def __init__(self, superregions): 
        self.superregions = superregions
        
    def _get_subregion_counts(self, region, normed): 
        for axis in normed.axes.values(): 
            if not axis.type == 'integral': 
                axis.integrate(reverse=True)
        kin = region['kinematics']
        met_cut = kin['met_gev']*1e3
        lead_cut = kin['leading_jet_gev']*1e3

        lslice, lext = normed.axes['leadingJetPt'].get_slice(lead_cut*0.999)
        mslice, mext = lslice.axes['met'].get_slice(met_cut*0.999)
        print lext, mext, mslice.array.shape, mslice.axes
        

    def extract_counts(self, h5_file_name): 
        if not isfile(h5_file_name): 
            raise IOError("{} doesn't exist".format(h5_file_name))
        with File(h5_file_name) as h5: 
            for k in h5: print k
            for supername, superregion in self.superregions.iteritems(): 
                normed = HistNd(h5[supername]['kinematics'])
                for name, region in superregion['subregions'].iteritems(): 
                    print name
                    self._get_subregion_counts(region, normed)
                    

                                

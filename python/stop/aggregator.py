from stop import meta
from stop.hists import HistNd
from os.path import basename, splitext
import h5py

class SampleAggregator(object): 
    """
    Takes some meta data and whiskey as an input, produces aggrigated 
    histograms. 
    
    At some point this should be given a write() method. 
    """
    def __init__(self, meta_path, whiskey, variables): 
        self.whiskey = whiskey
        self.variables = variables
        self.filter_meta = meta.DatasetCache(meta_path)
        self.lumi_fb = 15.0

    def aggregate(self): 
        plots_dict = {}
        all_cuts_used = set()
        for f in self.whiskey: 
            meta_name = basename(splitext(f)[0])
            if meta_name not in self.filter_meta: 
                continue
    
            file_meta = self.filter_meta[meta_name]
            if file_meta.bugs: 
                print "uh oh, bugs: {} in {}".format(
                    file_meta.bugs, meta_name)
                continue
    
            physics_type = file_meta.physics_type
            if physics_type == 'data': 
                lumi_scale = 1.0
            else: 
                lumi_scale = (self.lumi_fb / 
                              file_meta.effective_luminosity_fb )
    
            with h5py.File(f) as hfile: 
                for variable in self.variables: 
                    for cut_name, h5hist in hfile[variable].iteritems(): 
                        hist = HistNd(h5hist)
                        hist *= lumi_scale
                        idx_tuple = (physics_type, variable, cut_name)
                        if not idx_tuple in plots_dict: 
                            plots_dict[idx_tuple] = hist
                        else: 
                            plots_dict[idx_tuple] += hist
                        all_cuts_used.add(cut_name)

        self.all_cuts_used = all_cuts_used
        self.plots_dict = plots_dict

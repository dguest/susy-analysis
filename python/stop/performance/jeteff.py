import glob
import h5py
from stop.hist import HistNd

class JetEfficiencyPlotter(object): 
    _group_name = 'alljet'
    def __init__(self): 
        self.group_name = self._group_name 

    def _process_hist(self, hist_nd): 
        units = hist_nd.axes['x'].units
        raw_array, extent = hist_nd.project_1d('x')
        factor = {'MeV':1e-3, 'GeV':1.0}[units]
        extent = [e*factor for e in extent]
        return raw_array[1:-1], extent

    def _get_plots(self, hist_group, tags): 
        all_hist = HistNd(hist_group['all'])
        tags = {tag: HistNd(hist_group[tag]) for tag in tags}
        all_array, all_ext = self._process_hist(all_hist)
        plots = {}
        for tag, num in tags.iteritems(): 
            num_array, num_ext = self._process_hist(num)
            plots[tag] = ' **** you gotta do work here ****'
        

    def overlay_samples(self, sample_names): 
        

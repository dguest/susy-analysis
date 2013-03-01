from stop import meta
from stop.hists import HistNd
from os.path import basename, splitext
import h5py
import warnings
import yaml

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
        self.plots_dict = None
        self.tolerable_bugs = set([
                'bad files', 
                'ambiguous dataset', 
                ])

    def _get_objects(self, h5_cont): 
        objects = {}
        for name, val in h5_cont.items(): 
            if isinstance(val, h5py.Group): 
                objects[str(name)] = self._get_objects(val) 
            else: 
                objects[str(name)] = list(val.shape)
        return objects

    def yaml_objects(self): 
        with h5py.File(self.whiskey[0]) as hfile: 
            objects = self._get_objects(hfile)
        return yaml.dump(objects)

    def _check_for_bugs(self, ds): 
        if ds.bugs: 
            if ds.bugs - self.tolerable_bugs:
                print "uh oh, bugs: {} in {}".format(
                    ds.bugs, ds.key)
                return True
            if 'bad files' in ds.bugs: 
                try: 
                    nbad = ds.n_corrupted_files
                except AttributeError: 
                    nbad = 'unknown number of'
                warnings.warn('{} bad files in {}'.format(nbad,ds.key))
                return False
            if 'ambiguous dataset' in ds.bugs: 
                if ds.is_data: 
                    warnings.warn('you should probably fix the ambi ds thing')
                    return False
                else: 
                    print 'ambiguous dataset in {}, skipping'.format(ds.key)
                    return True

    def aggregate(self): 
        plots_dict = {}
        for f in self.whiskey: 
            meta_name = basename(splitext(f)[0])
            if meta_name not in self.filter_meta: 
                continue
    
            file_meta = self.filter_meta[meta_name]
            if self._check_for_bugs(file_meta): 
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

        self.plots_dict = plots_dict

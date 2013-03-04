from stop import meta
from stop.hists import HistNd
from os.path import basename, splitext
import h5py
import warnings
import sys

def _get_objects(h5_cont): 
    objects = {}
    for name, val in h5_cont.items(): 
        if isinstance(val, h5py.Group): 
            objects[str(name)] = _get_objects(val) 
        else: 
            objects[str(name)] = list(val.shape)
    return objects

def get_all_objects(filepath): 
    with h5py.File(filepath) as hfile: 
        objects = _get_objects(hfile)
    return objects


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
        self.signals = [ 
            dict(stop_mass_gev=200, lsp_mass_gev=180), 
            ]
        self.signal_prestring = 'stop'
        self._plots_dict = None
        self.tolerable_bugs = set([
                'bad files', 
                'ambiguous dataset', 
                'no filter efficiency'
                ])
        self.signal_name_template_met_filter = ( 
            '{}-{stop_mass_gev}-{lsp_mass_gev}-TMF{met_filter_gev:.0f}')
        self.signal_name_template = '{}-{stop_mass_gev}-{lsp_mass_gev}'
        self.outstream = sys.stdout

    def _get_matched_signame(self,ds): 
        """
        this is slightly hackish, but does the right thing by renaming the 
        physics type
        """
        basic_keys = ['stop_mass_gev','lsp_mass_gev']
        basic_generator_info = {k:ds.generator_info[k] for k in basic_keys}
        for sig in self.signals: 
            if sig == basic_generator_info: 
                if 'met_filter_gev' in ds.generator_info: 
                    namestring = self.signal_name_template_met_filter
                else: 
                    namestring = self.signal_name_template
                return namestring.format(self.signal_prestring,
                                         **ds.generator_info)
        return None

    def _check_for_bugs(self, ds): 
        if ds.bugs: 
            intolerable_bugs = ds.bugs - self.tolerable_bugs
            if intolerable_bugs:
                self.outstream.write("\nuh oh, bugs: {} in {} {}\n".format(
                    str(intolerable_bugs), ds.key, ds.name))
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

    @property
    def plots_dict(self): 
        """
        keys are tuples: (physics_type, variable, cut_name)
        """
        return self._plots_dict

    def aggregate(self): 
        plots_dict = {}
        numfiles = len(self.whiskey)
        for filenum, f in enumerate(self.whiskey): 
            if self.outstream and self.outstream.isatty():
                self.outstream.write(
                    '\radding file {} of {}'.format(filenum, numfiles))
                self.outstream.flush()
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

            if physics_type == 'signal': 
                physics_type = self._get_matched_signame(file_meta)
                if not physics_type: 
                    continue
    
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

        if self.outstream and self.outstream.isatty(): 
            self.outstream.write('\n')
        self._plots_dict = plots_dict

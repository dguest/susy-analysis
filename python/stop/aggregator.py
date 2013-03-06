from stop import meta
from stop.hists import HistNd
from os.path import basename, splitext, isfile
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

class HistDict(dict): 
    """
    Dictionary with some methods for HDF5 persistence. 

    Keyed by (physics, variable, cut) tuples. 

    The persistent format is actually rather ugly: 
        - top dir is physics type
        - bottom dir is cut
        - all middle dirs are the variable name
    """
    def __init__(self, file_name=''): 
        if not file_name: 
            return None
        with h5py.File(file_name) as infile: 
            for path in self._list_paths(infile): 
                spl = path.split('/')
                physics = spl[1]
                variable = '/'.join(spl[2:-1])
                cut = spl[-1]
                nametup = (physics, variable, cut)
                hist = infile[path]
                self[nametup] = HistNd(hist)

    def _list_paths(self, group, prefix=''): 
        paths = []
        for name, val in group.items(): 
            newprefix = '/'.join([prefix, name])
            if isinstance(val, h5py.Group): 
                paths += self._list_paths(val, newprefix)
            else: 
                paths.append(newprefix)
        return paths

    def write(self, file_name): 
        if isfile(file_name): 
            raise IOError('refusing to overwrite {}'.format(file_name))
        with h5py.File(file_name,'w') as outfile: 
            for nametup, ndhist in self.iteritems():
                physics, variable, cut = nametup
                path = '/'.join([''] + list(nametup))
                ndhist.write_to(outfile, path)
    

class SampleAggregator(object): 
    """
    Takes some meta data and whiskey as an input, produces aggrigated 
    histograms. 
    """
    signal_name_template_met_filter = ( 
        '{}-{stop_mass_gev}-{lsp_mass_gev}-TMF{met_filter_gev:.0f}')
    signal_name_template = '{}-{stop_mass_gev}-{lsp_mass_gev}'

    def __init__(self,meta_path, whiskey, variables): 
        self.whiskey = whiskey
        self.variables = variables
        self.filter_meta = meta.DatasetCache(meta_path)
        self.lumi_fb = 20.661
        self.signals = [        # todo, probably change this to 'all'
            dict(stop_mass_gev=200, lsp_mass_gev=180), 
            ]
        self.signal_prestring = 'stop'
        self._plots_dict = HistDict()
        self.tolerable_bugs = set([
                'ambiguous dataset', 
                ])
        self.outstream = sys.stdout
        self.bugstream = sys.stderr
        n_uniq_vars = len(set(variables))
        n_vars = len(variables)
        if n_vars != n_uniq_vars: 
            repeats = []
            for v in set(variables): 
                if variables.count(v) != 1: 
                    repeats.append(v)
            raise ValueError('{} appears in inputs more than once'.format(
                    repeats))

    def _get_matched_signame(self,ds): 
        """
        this is slightly hackish, but does the right thing by renaming the 
        physics type. 
        
        The 'all' option for self.signals is even more hackish...
        """
        basic_keys = ['stop_mass_gev','lsp_mass_gev']
        basic_generator_info = {k:ds.generator_info[k] for k in basic_keys}
        for sig in self.signals: 
            if self.signals == 'all' or sig == basic_generator_info: 
                if 'met_filter_gev' in ds.generator_info: 
                    namestring = self.signal_name_template_met_filter
                else: 
                    namestring = self.signal_name_template
                return namestring.format(self.signal_prestring,
                                         **ds.generator_info)
        return None

    def _check_for_bugs(self, ds): 
        if not ds.total_xsec_fb and not ds.is_data: 
            self.bugstream.write(
                'no cross section for {} {}, skipping\n'.format(
                    ds.key, ds.name))
            return True
        if ds.n_corrupted_files: 
            self.bugstream.write(
                '{} bad files in {}\n'.format(ds.n_corrupted_files, ds.key))
            
        if ds.bugs: 
            intolerable_bugs = ds.bugs - self.tolerable_bugs
            if intolerable_bugs:
                self.bugstream.write("\nuh oh, bugs: {} in {} {}\n".format(
                    str(intolerable_bugs), ds.key, ds.name))
                return True

            if 'ambiguous dataset' in ds.bugs: 
                if ds.is_data: 
                    warnings.warn('you should probably fix the ambi ds thing')
                else: 
                    print 'ambiguous dataset in {}, skipping'.format(ds.key)
                    return True
        return False

    @property
    def plots_dict(self): 
        """
        keys are tuples: (physics_type, variable, cut_name)
        """
        return self._plots_dict

    def aggregate(self): 
        plots_dict = HistDict()
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

    def write(self, file_name): 
        self._plots_dict.write(file_name)
    def read(self, file_name): 
        self._plots_dict = HistDict(file_name)
                
                        
class MissingCacheError(IOError): 
    """
    This is supposed to get thrown if something isn't in the cache. 
    Not currently used. 
    """
    def __init__(self,variable, physics, what=''): 
        self.variable = variable
        self.physics = physics
        if not what: 
            what = 'var: {}, in {} sample missing'.format(variable, physics)
        super(MissingCacheError,self).__init__(what)

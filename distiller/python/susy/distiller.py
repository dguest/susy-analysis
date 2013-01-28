from stop.meta import DatasetCache
from susy import cutflow
import re, os, glob
from os.path import isdir, join

class Distiller(object): 
    """
    Builds distillates from D3PDs. 

    Designed to run over one meta file and produce 
    """
    def __init__(self, config): 
        self.out_dir = config.out_dir
        self.meta_info_path = config.meta_info_path
        self.base_flags = 'r'
        self.verbose = False
        if hasattr(config,'verbose') and config.verbose: 
            print 'running {} in verbose mode'.format(__name__)
            self.base_flags += 'v'
            self.verbose = True

        self.setup_work_area()

    def setup_work_area(self): 
        """
        This builds directories for output files. Called by init too. 
        """
        def supply_dir(dname): 
            if not isdir(dname): 
                os.makedirs(dname)
        supply_dir(self.out_dir)
        
    def find_input_files(self, path): 
        """
        maps out root files and adds the meta info to the 
        """
        with DatasetCache(self.meta_info_path) as cache: 
            for ds_id, ds in cache.iteritems(): 
                globstr = '{path}/*{ds_id}*/*.root*'.format(
                    path=path, ds_id=ds_id)
                files = glob.glob(globstr)
                ds.d3pds = files
                if not files and self.verbose:
                    print 'WARNING: no files found for ds {} with {}'.format(
                        ds.id, globstr)

    def _get_flags(self, ds): 
        flags = self.base_flags

        if ds.origin.startswith('data'): 
            flags += 'd'
            return flags

        atlfast_finder = re.compile('(_a[0-9]+)+')
        if atlfast_finder.search(ds.tags): 
            flags += 'f'
        if self._signal_finder(ds.name): 
            flags += 's'

        return flags

    def distill(self): 
        with DatasetCache(self.meta_info_path) as cache: 
            for ds_id, ds in cache.iteritems(): 
                flags = self._get_flags(ds)
                build_conditions = [
                    not ds.skim_path, 
                    ]
                skip_conditions = [ 
                    not ds.d3pds
                    ]
                if any(skip_conditions): 
                    if self.verbose: 
                        print 'skipping {}'.format(ds.id)
                    continue
                if any(build_conditions): 
                    skim_name = '{}.root'.format(ds.id)
                    ds.skim_path = join(self.out_dir, skim_name)
                    ds.distill_flags = flags
                    cut_counts = cutflow.cutflow(
                        input_files=ds.d3pds, 
                        run_number=ds.id, 
                        flags=ds.distill_flags, 
                        output_ntuple=ds.skim_path)
                    ds.n_raw_entries = dict(cut_counts)['total_events']
                    ds.cutflow = dict(cut_counts)
                elif self.verbose: 
                    print 'skipped {}'.format(ds.id)
                    
    def _signal_finder(self, name): 
        """
        parses the name part of a dataset looking for signal
        """
        newsig = 'stopToCharm' in name
        oldsig = 'directCC' in name
        return oldsig or newsig


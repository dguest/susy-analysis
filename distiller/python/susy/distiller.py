from stop.meta import DatasetCache
from susy import cutflow
import re, os, glob, sys
from os.path import isdir, join, isfile
from multiprocessing import Pool

def _run_distill(ds): 
    print 'starting {} {}'.format(ds.id,ds.name)
    grl = ''
    if hasattr(ds,'grl'): 
        grl = ds.grl
    try: 
        cut_counts = cutflow.cutflow(
            input_files=ds.d3pds, 
            run_number=ds.id, 
            flags=ds.distill_flags, 
            grl=grl, 
            output_ntuple=ds.skim_path)
        ds.n_raw_entries = dict(cut_counts)['total_events']
        ds.cutflow = [list(c) for c in cut_counts]
        ds.need_rerun = False
        if 'bad files' in ds.bugs: 
            ds.bugs.remove('bad files')
    except RuntimeError as er: 
        if 'bad files:' in str(er): 
            ds.bugs.add('bad files')
        else: 
            raise 
    print 'done with {}'.format(ds.name)
    return ds
    

class Distiller(object): 
    """
    Builds distillates from D3PDs. 

    Designed to run over one meta file and produce 
    """
    def __init__(self, config): 
        self.out_dir = config.out_dir
        self.meta_info_path = config.meta_info_path
        self.base_flags = ''
        self.verbose = False
        self.force_rebuild = False
        self.warn_stream = sys.stdout
        if hasattr(config,'verbose') and config.verbose: 
            print 'running {} in verbose mode'.format(__name__)
            self.base_flags += 'v'
            self.verbose = True

        self.setup_work_area()
        try: 
            self.grl = config.grl
            if self.grl and not isfile(self.grl): 
                raise IOError("GRL {} doesn't exist".format(self.grl))
        except AttributeError: 
            self.grl = ''
        try: 
            self.ncore = config.ncore
        except AttributeError: 
            self.ncore = 1

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
            for ds_key, ds in cache.iteritems(): 
                globstr = '{path}/*{origin}*{ds_id}*/*.root*'.format(
                    path=path, origin=ds.origin, ds_id=ds.id)
                files = glob.glob(globstr)
                ds.d3pds = files
                if files and 'no d3pds' in ds.bugs: 
                    ds.bugs.remove('no d3pds')
                if not files and self.verbose:
                    self.warn_stream.write(
                        'WARNING: no files found for ds {} with {}\n'.format(
                            ds.id, globstr))

    def _get_flags(self, ds): 
        """
        get ds flags (translated internally to bitmask)
        """
        flags = self.base_flags

        if ds.origin.startswith('data'): 
            flags += 'd'
            return flags

        atlfast_finder = re.compile('(_a[0-9]+)+')
        if atlfast_finder.search(ds.tags): 
            flags += 'f'
        if self._signal_finder(ds.name): 
            flags += 's'

        if not flags: 
            return '_'          # placeholder

        return flags

    def _needs_rerun(self, ds): 
        """
        check a dataset for rerunning, based on age of input and output
        files. May add to bug set if inputs are missing. 
        """
        if not ds.d3pds: 
            ds.bugs.add('no d3pds')
            return False
        for d3pd in ds.d3pds: 
            if not isfile(d3pd): 
                raise IOError('d3pd {} not found'.format(d3pd))
        
        if ds.origin.startswith('data'): 
            has_grl = hasattr(ds,'grl') and ds.grl
            if has_grl and ds.grl != self.grl: 
                return True

        if not isfile(ds.skim_path): 
            return True
        skim_mtime = os.stat(ds.skim_path).st_mtime
        if any(os.stat(d3pd).st_mtime > skim_mtime for d3pd in ds.d3pds): 
            return True
        
        if self.force_rebuild: 
            return True

        return False


    def prepare_dataset_meta(self):
        with DatasetCache(self.meta_info_path) as cache: 
            for ds_id, ds in cache.iteritems(): 
                skim_name = '{}.root'.format(ds.key)
                ds.skim_path = join(self.out_dir, skim_name)
                ds.distill_flags = self._get_flags(ds)
                ds.need_rerun = self._needs_rerun(ds)
                if ds.origin.startswith('data'): 
                    ds.grl = self.grl
                else: 
                    ds.grl = ''

    def distill(self): 
        with DatasetCache(self.meta_info_path) as cache: 
            if self.ncore == 1: 
                for ds_id, ds in cache.iteritems(): 
                    if ds.need_rerun: 
                        updated_ds = _run_distill(ds)
                        ds = updated_ds
            else: 
                pool = Pool(self.ncore)
                rerun_ds = [ds for ds in cache.values() if ds.need_rerun]
                filled_datasets = pool.map(_run_distill,rerun_ds)
                for ds in filled_datasets: 
                    cache[ds.key] = ds
                    
    def _signal_finder(self, name): 
        """
        parses the name part of a dataset looking for signal
        """
        newsig = 'stopToCharm' in name
        oldsig = 'directCC' in name
        return oldsig or newsig


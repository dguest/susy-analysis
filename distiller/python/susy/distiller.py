from stop.meta import DatasetCache, MetaTextCollector
from susy import cutflow
import re, os, glob, sys
from os.path import isdir, join, isfile, expanduser
from multiprocessing import Pool
from warnings import warn

def _run_distill(ds): 
    """
    Note: need to rework the interface between this and cutflow.cutflow
    """
    print '{} starting, type: {}'.format(ds.id,ds.name)
    grl = ''
    if hasattr(ds,'grl'): 
        grl = ds.grl
    btag_env = ''
    if hasattr(ds, 'btag_env'): 
        btag_env = ds.btag_env
    try: 
        systematic = ds.need_rerun
        syst_internal = 'NONE' if systematic == 'baseline' else systematic
        cut_counts = cutflow.cutflow(
            input_files=ds.d3pds, 
            flags=ds.distill_flags, 
            grl=grl, 
            systematic=syst_internal, 
            output_ntuple=ds.skim_paths[systematic], 
            btag_cal_file=btag_env, 
            cal_dir=ds.calibration_dir)

        if not hasattr(ds,'cutflow') or isinstance(ds.cutflow, list): 
            ds.cutflow = {}
        cut_list = [list(c) for c in cut_counts]

        cut_names = [n for n,c in cut_list]
        _read_err = 'read_errors'
        if _read_err in cut_names: 
            ds.read_errors = dict(cut_counts)[_read_err]
            del cut_list[cut_names.index(_read_err)]
        ds.cutflow[systematic] = cut_list

        ds.need_rerun = False
        if isinstance(cut_counts, cutflow.CorruptedCutflow): 
            ds.n_corrupted_files += len(ds.d3pds) - len(cut_counts.files_used)
        if hasattr(ds,'missing_branch'): 
            del ds.missing_branch
    except RuntimeError as er: 
        if 'bad file:' in str(er): 
            ds.n_corrupted_files = len(ds.d3pds)
            warn(str(er))
        elif 'missing branch:' in str(er): 
            ds.missing_branch = str(er).split(':')[-1].strip()
        else: 
            raise

    ds.meta_sources |= set(['distiller'])
    print '{} done'.format(ds.id)
    return ds
    

class Distiller(object): 
    """
    Builds distillates from D3PDs. 

    Designed to run over one meta file and produce 
    """
    base_systematic = 'baseline'
    def __init__(self, config): 
        self.out_dir = config.out_dir
        self.meta_info_path = config.meta_info_path
        self.base_flags = 'z'
        self.verbose = False
        self.force_rebuild = False
        self.warn_stream = sys.stdout
        self.btag_env = 'BTagCalibration.env'
        if hasattr(config,'verbose') and config.verbose: 
            print 'running {} in verbose mode'.format(__name__)
            self.base_flags += 'v'
            self.verbose = True
        
        self.calibration_dir = expanduser(config.calibration_dir)
        if not isdir(self.calibration_dir): 
            raise IOError("Calibration dir {} doesn't exist".format(
                    self.calibration_dir))
        self.grl = ''
        if hasattr(config, 'grl'):
            self.grl = config.grl
            if not isfile(join(self.calibration_dir,self.grl)): 
                raise IOError("GRL {} doesn't exist".format(self.grl))

        try: 
            self.ncore = config.ncore
        except AttributeError: 
            self.ncore = 1

        if hasattr(config,'aggressive') and config.aggressive: 
            self.base_flags += 'a'

        if hasattr(config,'i') and config.i: 
            self.base_flags += 'i'

    def _setup_work_area(self): 
        """
        This builds directories for output files. Called by distill. 
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
                if ds.d3pds: 
                    continue
                globstr = '{path}/*{origin}*{ds_id}*{name}*/*.root*'.format(
                    path=path, origin=ds.origin, ds_id=ds.id, name=ds.name)
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
        
        overlaping_sherpa_finder = re.compile('MassiveCBPt0_')
        sherpa_pt_range_finder = re.compile('Pt[0-9]+_[0-9]+')
        if overlaping_sherpa_finder.search(ds.name): 
            flags += 'h'
            if sherpa_pt_range_finder.search(ds.name): 
                raise IOError( 
                    "can't determine if {} needs pt overlap removal".format(
                        ds.full_name))

        if not flags: 
            return '_'          # placeholder

        return flags

    def _needs_rerun(self, ds, systematic): 
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
        if ds.is_data and systematic != self.base_systematic: 
            return False
        
        if ds.origin.startswith('data'): 
            has_grl = hasattr(ds,'grl') and ds.grl
            if has_grl and ds.grl != self.grl: 
                return systematic

        if not systematic in ds.skim_paths: 
            return systematic
        if not isfile(ds.skim_paths[systematic]): 
            return systematic
        skim_mtime = os.stat(ds.skim_paths[systematic]).st_mtime
        if any(os.stat(d3pd).st_mtime > skim_mtime for d3pd in ds.d3pds): 
            return systematic
        
        if self.force_rebuild: 
            return systematic

        return False


    def prepare_dataset_meta(self, systematic=None):
        if systematic == None: 
            systematic = self.base_systematic
        btag_env = join(self.calibration_dir, self.btag_env)
        grl = join(self.calibration_dir, self.grl)
        if not isfile(btag_env): 
            raise IOError("{} not found".format(btag_env))

        textmeta = MetaTextCollector() 
        
        with DatasetCache(self.meta_info_path) as cache: 
            for ds_id, ds in cache.iteritems(): 
                skim_name = '{}.root'.format(ds.key)
                ds.skim_paths[systematic] = join(self.out_dir, skim_name)
                ds.distill_flags = self._get_flags(ds)
                ds.need_rerun = self._needs_rerun(ds, systematic)
                ds.calibration_dir = self.calibration_dir
                ds.n_corrupted_files = 0
                if ds.is_data: 
                    ds.grl = grl
                    ds.btag_env = ''
                else: 
                    ds.grl = ''
                    ds.btag_env = btag_env

                # In data we need to look at textfiles, which 
                # give the original count of events in the susy tree. 
                # In MC, we also need the summed event weight, which 
                # has to be generated in the skim at the moment. 
                n_raw, sum_wt, n_corr = textmeta.get_recorded_events(
                    ds.d3pds, aggressive=('a' in self.base_flags))
                ds.n_raw_entries = n_raw
                ds.sum_event_weight = sum_wt
                ds.n_corrupted_files += n_corr

    def split_datasets(self, max_d3pd_per_job): 
        with DatasetCache(self.meta_info_path) as cache: 
            new_datasets = {}
            rm_keys = []
            for ds in cache.itervalues(): 
                n_d3pd = len(ds.d3pds)
                if n_d3pd > max_d3pd_per_job: 
                    spl_num = (n_d3pd // max_d3pd_per_job) + 1
                    new_datasets.update(ds.split(spl_num))
                    rm_keys.append(ds.key)

            cache.update(new_datasets)
            for key in rm_keys: 
                del cache[key]

    def distill(self): 
        self._setup_work_area()
        with DatasetCache(self.meta_info_path) as cache: 
            if self.ncore == 1: 
                for ds_id, ds in cache.iteritems(): 
                    if ds.need_rerun: 
                        updated_ds = _run_distill(ds)
                        ds = updated_ds
            else: 
                pool = Pool(self.ncore)
                rerun_ds = [ds for ds in cache.values() if ds.need_rerun]
                filled_datasets = pool.map(_run_distill, rerun_ds)
                for ds in filled_datasets: 
                    cache[ds.key] = ds
                    
    def _signal_finder(self, name): 
        """
        parses the name part of a dataset looking for signal
        """
        newsig = 'stopToCharm' in name
        oldsig = 'directCC' in name
        return oldsig or newsig


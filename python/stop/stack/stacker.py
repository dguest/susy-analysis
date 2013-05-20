import sys, os
from os.path import join, isdir, basename, splitext, isfile
from stop import meta

class Stacker(object): 
    """
    Constructed  with a regions dict. Runs the stacking routine when 
    hist_from_ntuple is called.

    Some calibration is applied: if the mc_mc_sf_file is given all tagged 
    Sherpa jets will be rescaled. 
    """
    data_prepend = 'dme'
    mc_prepend = 's'
    def __init__(self, regions_dict, meta_dict=None): 
        self._regions = regions_dict
        self.dummy = False
        self.flags = set()
        self._verbose = False
        self.total_ntuples = None
        self.outstream = sys.stdout
        self.bugstream = sys.stderr
        self.rerun = False
        self.make_dirs = False
        self.mc_mc_sf_file = ''
        self.meta_dict = None
        if meta_dict: 
            self.meta_dict = meta.DatasetCache(meta_dict)
    @property
    def verbose(self): 
        return self._verbose
    @verbose.setter
    def verbose(self, value): 
        self._verbose = value
        if value:
            self.flags |= set('v')
        else: 
            self.flags -= set('v')

    def _ismc(self, ntuple_name): 
        first = ntuple_name.split('/')[-1][0]
        if first in self.data_prepend: 
            return False
        elif first in self.mc_prepend: 
            return True
        else: 
            raise ValueError(
                "not sure what '{}' does as ntuple prepend in '{}'".format(
                    first, ntuple_name))

    def _setup_region_dict(self, name, reg, outsubdir, systematic, ntuple, 
                           basedir): 
        regdic = reg.get_config_dict()
        modedir = regdic['hists'].lower()
        regdic['name'] = name
        if outsubdir: 
            systdir = outsubdir
        else: 
            if systematic == 'NONE': 
                systdir = 'baseline'
            else: 
                systdir = systematic.lower()
        full_out_dir = join(basedir, modedir, systdir)
        if not isdir(full_out_dir): 
            if self.make_dirs: 
                os.makedirs(full_out_dir)
            else: 
                raise IOError(99,"no dir",full_out_dir)
        histname = '{}.h5'.format(basename(splitext(ntuple)[0]))
        full_out_path = join(full_out_dir, histname)
        if isfile(full_out_path): 
            if self.rerun: 
                os.remove(full_out_path)
            else: 
                return None
        regdic['output_name'] = full_out_path
        regdic['systematic'] = systematic


        return regdic

    def run_multisys(self, ntuple, basedir, systematics, tuple_n=None, 
                     outsubdir=None): 
        regions = []
        for name, reg in self._regions.items(): 
            for systematic in systematics: 
                if self._ismc(ntuple) or systematic == 'NONE':
                    regdic = self._setup_region_dict(
                        name, reg, outsubdir, systematic, ntuple, basedir)
                    if regdic: 
                        regions.append(regdic)

        if regions: 
            self._print_progress(tuple_n, systematics, outsubdir)
            self._run(ntuple, regions)
            return 1
        else: 
            return 0

    def _print_progress(self, tuple_n, systematics, outsubdir): 
        print_req = [
            not self.verbose, 
            self.outstream.isatty(), 
            isinstance(tuple_n,int), 
            self.total_ntuples, 
            ]
        if all(print_req):
            n_syst = len(systematics)
            if n_syst == 1: 
                if outsubdir: 
                    tag = outsubdir
                else: 
                    tag = systematics[0]
                prline = '\rstacking {} ({:4} of {})'.format(
                    tag, tuple_n, self.total_ntuples)
            else: 
                prline = '\rstacking {} systematics ({:4} of {})'.format(
                    n_syst, tuple_n,self.total_ntuples)
            self.outstream.write(prline)
            self.outstream.flush()

    def _run(self, ntuple, regions): 
        if self.dummy: 
            print ntuple
            for reg in regions: 
                print reg
            return 
        from stop.hyperstack import stacksusy
        flags = ''.join(self.flags)
        if basename(ntuple).startswith('d'): 
            flags += 'd'
        if not isfile(ntuple): 
            raise IOError(3,"doesn't exist",ntuple)

        # calibration for sherpa jets is hacked in here. 
        if self.mc_mc_sf_file: 
            ntuple_key = basename(splitext(ntuple)[0]).split('-')[0]
            ds_name = self.meta_dict[ntuple_key].name
            if 'Sherpa' in ds_name: 
                mcfile = self.mc_mc_sf_file
                for region in regions: 
                    region['mc_mc_jet_reweight_file'] = mcfile

        stacksusy(ntuple, regions, flags=flags)
        

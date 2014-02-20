import sys, os
from os.path import join, isdir, basename, splitext, isfile
from scharm import schema
from scharm.bullshit import make_dir_if_none

class Stacker(object): 
    """
    Constructed  with a regions dict. Runs the stacking routine when 
    hist_from_ntuple is called.
    """
    data_prepend = 'jem'
    mc_prepend = 'sa'
    def __init__(self, regions_dict, base_dir): 
        self._regions = regions_dict
        self.dummy = False
        self.flags = set()
        self._verbose = False
        self.total_ntuples = None
        self.outstream = sys.stdout
        self.bugstream = sys.stderr
        self.rerun = False
        self.make_dirs = False
        self.base_dir = base_dir
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

    def _setup_region_dict(self, name, reg, ntuple, systematic): 
        regdic = reg.get_config_dict()
        regdic['name'] = name
        dist_settings = schema.distiller_settings_from_dir(dirname(ntuple))
        dist_settings['systematic'] = systematic
        mid_path = schema.dir_from_distiller_settings(
            dist_settings, base=self.base_dir)
        full_out_dir = join(mid_path, regdic['hists'].lower())
        if not isdir(full_out_dir): 
            if self.make_dirs: 
                make_dir_if_none(full_out_dir)
            else: 
                raise IOError(99,"no dir",full_out_dir)
        histname = '{}.h5'.format(basename(splitext(ntuple)[0]))
        full_out_path = join(full_out_dir, histname)
        if isfile(full_out_path): 
            os.remove(full_out_path)
        regdic['output_name'] = full_out_path
        regdic['systematic'] = systematic.upper()

        return regdic

    def run_multisys(self, ntuple, systematics, tuple_n=None): 
        regions = []
        for name, reg in self._regions.items(): 
            for systematic in systematics: 
                if self._ismc(ntuple) or systematic == 'NONE':
                    regdic = self._setup_region_dict(
                        name, reg, ntuple, systematic)
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
            print(ntuple)
            for reg in regions: 
                print(reg)
            return 
        from stop.stack.hfw import stacksusy
        flags = ''.join(self.flags)
        if basename(ntuple).startswith('d'): 
            flags += 'd'
        if not isfile(ntuple): 
            raise IOError(3,"doesn't exist",ntuple)

        stacksusy(ntuple, regions, flags=flags)
        

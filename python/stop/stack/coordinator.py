import yaml
from stop import bits
from stop.stack.regions import Region, condense_regions
from os.path import basename, isfile, isdir, join, splitext
import glob, os, tempfile, sys

class Coordinator(object): 
    """
    Constructed with a file-like object. Fills out the steering file, then
    can (re)run the d3pd -> hist (stacker) / aggregator routines. 

    This class should handle all the yaml parsing stuff. 

    General Design Principals: 
        - missing directories are generated when needed. 
        - a template yaml file should be generated if none is given. 
        - this class (and only this class) is responsible for checking the 
          consistency of the yaml file. 
        - This class doesn't do plotting. 
    """
    distiller_systematics = [
        'NONE','JESUP', 'JESDOWN'
        ]
    scale_factor_systematics = ['NONE'] 
    scale_factor_systematics += [
        part + shift for part in 'BCUT' for shift in ['UP','DOWN']
        ]
    aggregate_hist_name = 'aggregate.h5'

    def __init__(self, yaml_file=None): 
        if yaml_file: 
            self._config_dict = yaml.load(yaml_file)
        else: 
            self._config_dict = {}
        if not 'regions' in self._config_dict: 
            if yaml_file: 
                self.bugstream.write('no regions given, making default')
            example = Region().get_yaml_dict()
            self._config_dict['regions'] = {'EXAMPLE':example}
        if not 'files' in self._config_dict: 
            if yaml_file: 
                self.bugstream.write('no files given, making default')
            self._config_dict['files'] = {
                'ntuples': {s:s.lower() for s in self.distiller_systematics}, 
                'hists': 'hists', 
                'meta':'meta-all.yml', 
                'plots':'plots'}
        if not 'misc' in self._config_dict: 
            if yaml_file: 
                self.bugstream.write('no misc info, making default')
            self._config_dict['misc'] = {'lumi_fb': 20.661}

        self.verbose = False
        self.outstream = sys.stdout
        self.bugstream = sys.stderr

    def __repr__(self): 
        return repr(self._config_dict)
    
    def _dirify(self, systematic): 
        return 'baseline' if systematic == 'NONE' else systematic.lower()

    def _get_syst_hist_path(self, systematic, create=False): 
        hists_path = self._config_dict['files']['hists']
        syst_hist_path = join(hists_path, self._dirify(systematic))
        if create:
            if not isdir(hists_path): 
                os.mkdir(hists_path)
            if not isdir(syst_hist_path): 
                os.mkdir(syst_hist_path)
        return syst_hist_path

    def _print_new_line(self): 
        if not self.verbose and self.outstream.isatty(): 
            self.outstream.write('\n')

    def get_plotting_meta(self): 
        meta = {
            'lumi_fb': self._config_dict['misc']['lumi_fb'], 
            'base_dir': self._config_dict['files']['plots'], 
            }
        return meta

    def clean(self): 
        hists_dir = self._config_dict['files']['hists']
        if isdir(hists_dir): 
            for syst in self.distiller_systematics: 
                histsyst_dir = join(hists_dir, self._dirify(syst))
                if isdir(histsyst_dir): 
                    for f in glob.glob('{}/*.h5'.format(histsyst_dir)): 
                        os.remove(f)
                    os.rmdir(histsyst_dir)
            os.rmdir(hists_dir)

    def get_broken_regions(self): 
        broken = []
        for name, region_dict in self._config_dict['regions'].items(): 
            try: 
                region = Region(region_dict)
                bits = region.get_bits()
                antibits = region.get_antibits()
            except KeyError: 
                broken.append(name)
                continue
            except ValueError: 
                broken.append(name)
        return broken

    def _fast_stack(self, rerun=False, do_stat_regions=False): 
        ntup_dict = self._config_dict['files']['ntuples']
        globdir = '{}/*.root'.format(ntup_dict['NONE'])
        ntuples = glob.glob(globdir)
        if not ntuples: 
            raise IOError("no ntuples found in {}".format(globdir))

        for syst in self.scale_factor_systematics: 
            hist_path = self._get_syst_hist_path(syst, create=True)

        base_hist_path = '/'.join(hist_path.split('/')[:-1])
        stacker_regions = {
            k:Region(v) for k,v in self._config_dict['regions'].items()}
        if do_stat_regions: 
            stacker_regions = condense_regions(stacker_regions)
        stacker = Stacker(stacker_regions)
        stacker.total_ntuples = len(ntuples)
        stacker.rerun = rerun
        n_ran = 0
        for tup_n, ntup in enumerate(ntuples): 
            n_ran += stacker.run_multisys(
                ntup, base_hist_path, 
                self.scale_factor_systematics, 
                tuple_n=tup_n)
        if n_ran: 
            self._print_new_line()
            n_ran = 0

        dist_syst = set(self.distiller_systematics) 
        dist_syst -= set(['NONE'])
        for syst in dist_syst: 
            self.stack(systematic=syst, rerun=rerun, 
                       do_stat_regions=do_stat_regions)
        if n_ran: 
            self._print_new_line()

    def stack(self, systematic='NONE', rerun=False, do_stat_regions=False): 
        if systematic == 'all': 
            self._fast_stack(rerun, do_stat_regions)
            return 
                
        ntup_dict = self._config_dict['files']['ntuples']
        if systematic in self.distiller_systematics: 
            globdir = ntup_dict[systematic]
            stacker_syst = 'NONE'
        elif systematic in self.scale_factor_systematics: 
            globdir = ntup_dict['NONE']
            stacker_syst = systematic
        else: 
            raise ValueError('what the fuck is {}'.format(systematic))
        ntuples = glob.glob('{}/*.root'.format(globdir))
        if not ntuples: 
            raise IOError("no ntuples found in {}".format(globdir))

        syst_hist_path = self._get_syst_hist_path(systematic, create=True)
        stacker_regions = {
            k:Region(v) for k,v in self._config_dict['regions'].items()}
        if do_stat_regions: 
            stacker_regions = condense_regions(stacker_regions)
        stacker = Stacker(stacker_regions)
        stacker.total_ntuples = len(ntuples)
        stacker.rerun = rerun
        n_ran = 0
        for tup_n, ntup in enumerate(ntuples): 
            out_hist_name = '{}.h5'.format(splitext(basename(ntup))[0])
            out_hist_path = join(syst_hist_path, out_hist_name)
            n_ran += stacker.hist_from_ntuple(
                ntup, out_hist_path, stacker_syst,
                tuple_n=tup_n)
        if n_ran: 
            self._print_new_line()

    def get_needed_aggregates(self): 
        """
        returns a list of systematics
        """
        all_syst = set(self.distiller_systematics + 
                       self.scale_factor_systematics)
        needed = []
        for syst in all_syst: 
            agg_name = join(self._get_syst_hist_path(syst), 
                            self.aggregate_hist_name)
            if not isfile(agg_name): 
                needed.append(syst)
        return needed

    def aggregate(self, systematic='NONE', rerun=False, variables='all'): 
        if systematic == 'all': 
            all_syst = set(self.distiller_systematics + 
                           self.scale_factor_systematics)
            syst_dict = {}
            for syst in all_syst: 
                self.outstream.write(
                    'aggregating systematic {}\n'.format(syst))
                syst_dict[syst] = self.aggregate(systematic=syst, 
                                                 rerun=rerun)
            return syst_dict
            
        from stop.stack import aggregator as agg
        hist_path = self._get_syst_hist_path(systematic)
        agg_name = join(hist_path, self.aggregate_hist_name)
        if isfile(agg_name) and not rerun: 
            hist_dict = agg.HistDict(agg_name)
        else: 
            whiskey = glob.glob('{}/*.h5'.format(hist_path))
            aggregator = agg.SampleAggregator(
                meta_path=self._config_dict['files']['meta'], 
                whiskey=whiskey, 
                variables=variables,
            )
            aggregator.lumi_fb = self._config_dict['misc']['lumi_fb']
            aggregator.signals = 'all'
            aggregator.bugstream = tempfile.TemporaryFile()
            aggregator.out_prepend = 'systamatic: {} '.format(systematic)
            aggregator.aggregate()
            aggregator.bugstream.seek(0)
            for line in aggregator.bugstream: 
                if not aggregator.bugstream.isatty(): 
                    line = 'syst {}: {}'.format(systematic, line)
                self.bugstream.write(line)
            if isfile(agg_name): 
                os.remove(agg_name)
            aggregator.write(agg_name)
            hist_dict = aggregator.plots_dict
        return hist_dict
            
    def write(self, yaml_file): 
        for line in yaml.dump(self._config_dict): 
            yaml_file.write(line)


# -- consider moving this ?
class Stacker(object): 
    """
    Constructed  with a regions dict. Runs the stacking routine when 
    hist_from_ntuple is called.
    """
    def __init__(self, regions_dict): 
        self._regions = regions_dict
        self.dummy = False
        self.flags = set()
        self._verbose = False
        self.total_ntuples = None
        self.outstream = sys.stdout
        self.bugstream = sys.stderr
        self.rerun = False
    @property
    def verbose(self): 
        return self._verbose
    @verbose.setter
    def verbose(self, value): 
        self._verbose = value
        if value:
            self.flags += set('v')
        else: 
            self.flags -= set('v')
            
    
    def hist_from_ntuple(self, ntuple, hist, systematic='NONE', 
                         tuple_n=None): 
        # got to figure out how I'm going to deal with possibly 
        # differing configuration cuts... 
        # current plan, don't configure cuts, output a histogram and 
        # do the counting on that... we'll use stacksusy as a standin
        if isfile(hist):
            if self.rerun: 
                os.remove(hist)
            else: 
                return 0

        regions = []
        for name, reg in self._regions.items(): 
            regdic = reg.get_config_dict()
            regdic['name'] = name
            regdic['output_name'] = hist 
            regdic['systematic'] = systematic
            regions.append(regdic)


        print_req = [
            not self.verbose, 
            self.outstream.isatty(), 
            isinstance(tuple_n,int), 
            self.total_ntuples, 
            ]
        if all(print_req):
            sp_path = hist.split('/')
            if len(sp_path) > 1: 
                sysname = sp_path[-2].upper()
            else: 
                sysname = systematic
            prline = '\rstacking systematic {} ({:4} of {})'.format(
                sysname, tuple_n, self.total_ntuples)
            self.outstream.write(prline)
            self.outstream.flush()

        
        self._run(ntuple, regions)
        return 1

    def run_multisys(self, ntuple, basedir, systematics, tuple_n=None): 
        regions = []
        for name, reg in self._regions.items(): 
            for systematic in systematics: 
                regdic = reg.get_config_dict()
                regdic['name'] = name
                if systematic == 'NONE': 
                    outdir = 'baseline'
                else: 
                    outdir = systematic.lower()
                full_out_dir = join(basedir, outdir)
                if not isdir(full_out_dir): 
                    raise IOError(99,"no dir",full_out_dir)
                histname = '{}.h5'.format(basename(splitext(ntuple)[0]))
                full_out_path = join(full_out_dir, histname)
                if isfile(full_out_path): 
                    if self.rerun: 
                        os.remove(full_out_path)
                    else: 
                        continue
                regdic['output_name'] = full_out_path
                regdic['systematic'] = systematic
                regions.append(regdic)

        if regions: 

            print_req = [
                not self.verbose, 
                self.outstream.isatty(), 
                isinstance(tuple_n,int), 
                self.total_ntuples, 
                ]
            if all(print_req):
                n_syst = len(systematics)
                prline = '\rstacking {} systematics ({:4} of {})'.format(
                    n_syst, tuple_n,self.total_ntuples)
                self.outstream.write(prline)
                self.outstream.flush()
            self._run(ntuple, regions)
            return 1
        else: 
            return 0

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
        stacksusy(ntuple, regions, flags=flags)

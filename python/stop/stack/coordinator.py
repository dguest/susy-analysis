import yaml
from stop import bits
from stop.stack import table, extractor
from stop.stack.regions import Region, condense_regions
from stop.stack.stacker import Stacker
from os.path import basename, isfile, isdir, join, splitext
import glob, os, tempfile, sys
import warnings

class Coordinator(object): 
    """
    Constructed with a file-like object. Fills out the steering file, then
    can (re)run the ntuple -> hist (stacker) / aggregator routines. 

    This class should handle all the yaml parsing stuff. 

    General Design Principals: 
        - missing directories are generated when needed. 
        - a template yaml file should be generated if none is given. 
        - this class (and only this class) is responsible for checking the 
          consistency of the yaml file. 
        - This class doesn't do plotting. 
    """
    distiller_systematics = [
        'NONE','JESUP', 'JESDOWN', 'JER', 
        ]
    scale_factor_systematics = ['NONE'] 
    scale_factor_systematics += [
        part + shift for part in 'BCUT' for shift in ['UP','DOWN']
        ]
    aggregate_hist_name = 'aggregate.h5'
    super_region_meta_name = 'superregions.yml'

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
                'hadd-hists':'hadd-hists', 
                'meta':'meta-all.yml', 
                'counts':'counts.yml', 
                'mc_mc_sf_file': 'mc-mc-sf.txt', 
                'plots':'plots'}
        if not 'misc' in self._config_dict: 
            if yaml_file: 
                self.bugstream.write('no misc info, making default')
            self._config_dict['misc'] = {'lumi_fb': 20.661}
        if not 'backgrounds' in self._config_dict: 
            self._config_dict['backgrounds'] = {
                'used': ['EXAMPLE_BG'], 
                'variations': {
                    'EXAMPLE_BG': {
                        'EXAMPLE_SYSTEMATIC': {
                            'base':'EXAMPLE_BG', 
                            'variation':'EXAMPLE_BG', 
                            'factor': 1.0}
                        } 
                    }
                }

        self.verbose = False
        self.outstream = sys.stdout
        self.bugstream = sys.stderr

    def __repr__(self): 
        return repr(self._config_dict)
    
    def _dirify(self, systematic): 
        return 'baseline' if systematic == 'NONE' else systematic.lower()

    def _make_hist_path(self, systematic='', mode='' , create=False): 
        hists_path = self._config_dict['files']['hadd-hists']
        if not systematic and mode: 
            systematic = 'baseline'
        elif systematic: 
            systematic = self._dirify(systematic)
        syst_hist_path = join(hists_path, mode, systematic)
        if create:
            if not isdir(hists_path): 
                os.makedirs(syst_hist_path)
        return syst_hist_path

    def _write_plot_region_meta(self, regions, overwrite=True): 
        hist_path = self._make_hist_path(create=True)
        yml_path = join(hist_path, self.super_region_meta_name)
        if isfile(yml_path): 
            if overwrite: 
                os.remove(yml_path)
            else: 
                raise IOError("{} already exists, won't overwrite".format(
                        yml_path))
        yml_dict = {
            n:v.get_yaml_dict() for n,v in regions.iteritems()}
        with open(yml_path, 'w') as yml: 
            yml.write(yaml.dump(yml_dict))

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
        hists_dir = self._config_dict['files']['hadd-hists']
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

    def _fast_stack(self, rerun=False, mode='histmill'): 
        """
        TODO: unify this with stack() 
        """
        ntup_dict = self._config_dict['files']['ntuples']
        globdir = '{}/*.root'.format(ntup_dict['NONE'])
        ntuples = glob.glob(globdir)
        if not ntuples: 
            raise IOError("no ntuples found in {}".format(globdir))

        base_hist_path = self._make_hist_path()
        stacker_regions = {
            k:Region(v) for k,v in self._config_dict['regions'].items()}
        if mode == 'kinematic_stat': 
            stacker_regions = condense_regions(stacker_regions)
            self._write_plot_region_meta(stacker_regions)
        stacker = Stacker(
            stacker_regions,
            meta_dict=self._config_dict['files']['meta'])
        stacker.mc_mc_sf_file = self._config_dict['files']['mc_mc_sf_file']
        stacker.total_ntuples = len(ntuples)
        stacker.rerun = rerun
        stacker.make_dirs = True
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
            self.stack(systematic=syst, rerun=rerun, mode=mode)
        if n_ran: 
            self._print_new_line()

    def stack(self, systematic='NONE', rerun=False, mode='histmill'): 
        warnings.warn(
            'this function should be removed, use susy-stack', 
            FutureWarning, stacklevel=2)
        if systematic == 'all': 
            self._fast_stack(rerun, mode)
            return 
                
        ntup_dict = self._config_dict['files']['ntuples']
        if systematic in self.distiller_systematics or systematic == 'NONE': 
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

        base_hist_path = self._make_hist_path(create=True)
        stacker_regions = {
            k:Region(v) for k,v in self._config_dict['regions'].items()}
        if mode == 'kinematic_stat': 
            stacker_regions = condense_regions(stacker_regions)
        stacker = Stacker(
            stacker_regions, 
            meta_dict=self._config_dict['files']['meta'])
        stacker.mc_mc_sf_file = self._config_dict['files']['mc_mc_sf_file']
        stacker.total_ntuples = len(ntuples)
        stacker.rerun = rerun
        stacker.make_dirs = True
        n_ran = 0
        for tup_n, ntup in enumerate(ntuples): 
            n_ran += stacker.run_multisys(
                ntup, base_hist_path, [stacker_syst],
                tuple_n=tup_n, outsubdir=self._dirify(systematic))
        if n_ran: 
            self._print_new_line()

    def get_needed_aggregates(self, mode): 
        """
        returns a list of systematics
        """
        all_syst = set(self.distiller_systematics + 
                       self.scale_factor_systematics)
        needed = []
        for syst in all_syst: 
            agg_name = join(self._make_hist_path(syst, mode), 
                            self.aggregate_hist_name)
            if not isfile(agg_name): 
                needed.append(syst)
        return needed

    def aggregate(self, systematic='NONE', rerun=False, variables='all', 
                  mode='histmill'): 
        if systematic == 'all': 
            all_syst = set(self.distiller_systematics + 
                           self.scale_factor_systematics)
            syst_dict = {}
            for syst in all_syst: 
                syst_dict[syst] = self.aggregate(systematic=syst, 
                                                 rerun=rerun, mode=mode)
            return syst_dict
            
        from stop.stack import aggregator as agg
        hist_path = self._make_hist_path(systematic, mode)
        agg_name = join(hist_path, self.aggregate_hist_name)
        if isfile(agg_name) and not rerun: 
            self.outstream.write('loading cached aggregate: {}\n'.format(
                    agg_name))
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

        if mode == 'kinematic_stat': 
            hist_path = self._make_hist_path()
            extractor_yml = join(hist_path, self.super_region_meta_name)
            with open(extractor_yml) as yml: 
                ext = extractor.RegionExtractor(yaml.load(yml))
            regions = self._config_dict['regions']
            normed = ext.extract_counts_dict(hist_dict, regions, 
                                             variable='kinematics')
            wt_2 = ext.extract_counts_dict(hist_dict, regions, 
                                           variable='kinematicWt2')
            def combine(k): 
                if k in wt_2: 
                    if wt_2[k]: 
                        eff_stats = normed[k]**2.0 / wt_2[k]
                    else: 
                        eff_stats = 0.0
                    return {
                        'normed': normed[k], 
                        'stats': eff_stats, 
                        }
                else:
                    return normed[k]

            comb = {k:combine(k) for k in normed}
            return comb
        else: 
            return None
            
    def write(self, yaml_file): 
        for line in yaml.dump(self._config_dict): 
            yaml_file.write(line)


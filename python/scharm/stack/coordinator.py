import yaml
from stop import bits
from stop.stack import table, extractor
from stop.stack.regions import Region, condense_regions
from stop.stack.stacker import Stacker
from os.path import basename, isfile, isdir, join, splitext
import glob, os, tempfile, sys
import warnings


def _get_systematics(base):
    systs = [d.upper() for d in os.listdir(base) if isdir(join(base,d))]
    if not systs:
        raise ValueError('{} : {} in {}'.format(err, systs, base))
    return systs


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
    shift_sf = list('BCUT') + ['EL','MU']
    scale_factor_systematics += [
        part + shift for part in shift_sf for shift in ['UP','DOWN']
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

    def _get_superregion_yaml(self):
        def checkpath(place):
            the_dir = self._config_dict['files'][place]
            the_path = join(the_dir, self.super_region_meta_name)
            if isfile(the_path):
                return the_path
        primary = checkpath('hadd-hists')
        if primary:
            return primary
        secondary = checkpath('hists')
        if secondary:
            return secondary
        raise OSError('no {} found'.format(self.super_region_meta_name))

    def aggregate(self, systematic='baseline', rerun=False, variables='all',
                  mode='histmill'):
        if systematic == 'all':
            all_syst = _get_systematics(
                join(self._config_dict['files']['hadd-hists'], mode))
            all_syst = ['none' if s == 'baseline' else s for s in all_syst]
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
            # ACHTUNG: could probalby move this stuff to top level in
            # some script
            whiskey = glob.glob('{}/*.h5'.format(hist_path))
            aggregator = agg.SampleAggregator(
                meta_path=self._config_dict['files']['meta'],
                whiskey=whiskey,
                variables=variables,
            )
            aggregator.lumi_fb = self._config_dict['misc']['lumi_fb']
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
            extractor_yml = self._get_superregion_yaml()
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


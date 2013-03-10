import yaml
from stop import bits
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
    """
    distiller_systematics = [
        'NONE','JESUP', 'JESDOWN'
        ]
    scale_factor_systematics = ['NONE'] 
    scale_factor_systematics += [
        part + shift for part in 'BCUT' for shift in ['UP','DOWN']
        ]
    aggrigate_hist_name = 'aggregate.h5'

    def __init__(self, yaml_file=None): 
        if yaml_file: 
            self._config_dict = yaml.load(yaml_file)
        else: 
            self._config_dict = {}
        if not 'regions' in self._config_dict: 
            example = Region().get_yaml_dict()
            self._config_dict['regions'] = {'EXAMPLE':example}
        if not 'files' in self._config_dict: 
            self._config_dict['files'] = {
                'ntuples': {s:s.lower() for s in self.distiller_systematics}, 
                'hists': 'hists', 
                'meta':'meta-all.yml'}
        if not 'misc' in self._config_dict: 
            self._config_dict['misc'] = {'lumi_fb': 20.661}

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
                bits = region.bits
                antibits = region.antibits
            except KeyError: 
                broken.append(name)
                continue
            except ValueError: 
                broken.append(name)
        return broken

    def fast_stack(self, rerun=False): 
        ntup_dict = self._config_dict['files']['ntuples']
        ntuples = glob.glob('{}/*.root'.format(ntup_dict['NONE']))
        if not ntuples: 
            raise IOError("no ntuples found in {}".format(globdir))

        for syst in self.scale_factor_systematics: 
            hist_path = self._get_syst_hist_path(syst, create=True)

        base_hist_path = '/'.join(hist_path.split('/')[:-1])
        stacker = Stacker(self._config_dict['regions'])
        for ntup in ntuples: 
            stacker.run_multisys(
                ntup, base_hist_path, 
                self.scale_factor_systematics, rerun=rerun)

        dist_syst = set(self.distiller_systematics) 
        dist_syst -= set(['NONE'])
        for syst in dist_syst: 
            self.stack(systematic=syst, rerun=rerun)


    def stack(self, systematic='NONE', rerun=False): 
        if systematic == 'all': 
            self.fast_stack(rerun)
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
        stacker = Stacker(self._config_dict['regions'])
        for ntup in ntuples: 
            out_hist_name = '{}.h5'.format(splitext(basename(ntup))[0])
            out_hist_path = join(syst_hist_path, out_hist_name)
            if isfile(out_hist_path):
                if rerun: 
                    os.remove(out_hist_path)
                else: 
                    continue
            stacker.hist_from_ntuple(ntup, out_hist_path, stacker_syst)

    def aggregate(self, systematic='NONE', rerun=False): 
        if systematic == 'all': 
            all_syst = set(self.distiller_systematics + 
                           self.scale_factor_systematics)
            syst_dict = {}
            for syst in all_syst: 
                syst_dict[syst] = self.aggregate(systematic=syst, 
                                                 rerun=rerun)
            return syst_dict
            
        from stop.stack import aggregator as agg
        hist_path = self._get_syst_hist_path(systematic)
        agg_name = join(hist_path, self.aggrigate_hist_name)
        if isfile(agg_name) and not rerun: 
            hist_dict = agg.HistDict(agg_name)
        else: 
            whiskey = glob.glob('{}/*.h5'.format(hist_path))
            aggregator = agg.SampleAggregator(
                meta_path=self._config_dict['files']['meta'], 
                whiskey=whiskey, 
                variables=['met']
            )
            aggregator.lumi_fb = self._config_dict['misc']['lumi_fb']
            aggregator.signals = 'all'
            aggregator.bugstream = tempfile.TemporaryFile()
            aggregator.aggregate()
            aggregator.bugstream.seek(0)
            for line in aggregator.bugstream: 
                sys.stderr.write(line)
            if isfile(agg_name): 
                os.remove(agg_name)
            aggregator.write(agg_name)
            hist_dict = aggregator.plots_dict
        return hist_dict
            
    def write(self, yaml_file): 
        for line in yaml.dump(self._config_dict): 
            yaml_file.write(line)

class Region(object): 
    """
    Stores info on signal / control region. Bits are stored as strings 
    and used to look up real values. This class is also responsible for
    checking the integrity of its stored data. 
    """
    default_dict = { 
        'type':'control', 
        'req_bits':['preselection'], 
        'veto_bits':[], 
        'cut_config':{
            'leading_jet_gev':240, 
            'met_gev':180, 
            'btag_config':['NOTAG','LOOSE','TIGHT']
            }
        }
    _bit_dict = dict(bits.bits)
    _composite_bit_dict = dict(bits.composite_bits)
    _final_dict = bits.final_dict
    _allowed_types = set(['control','signal'])

    def __init__(self, yaml_dict={}): 
        if not yaml_dict: 
            yaml_dict = self.default_dict
        self.type = yaml_dict['type']
        self.req_bits = yaml_dict['req_bits']
        self.veto_bits = yaml_dict['veto_bits']
        self.cut_config = yaml_dict['cut_config']
        if self.type not in self._allowed_types: 
            raise ValueError('region type {} is not known'.format(
                    self.type))
    def __repr__(self): 
        return repr(self.get_yaml_dict())

    def get_yaml_dict(self): 
        """
        dumps the object as a dict for yaml
        """
        # as long as the names don't change we can just dump the object data
        baselist = self.__dict__.items()
        base = {k:v for k, v in baselist if not k.startswith('_')}
        return base

    def _get_bits(self, namelist): 
        bits = 0
        for name in namelist:
            if name in self._final_dict: 
                bits |= self._final_dict[name]
            elif name in self._composite_bit_dict: 
                bits |= self._composite_bit_dict[name]
            elif name in self._bit_dict: 
                bits |= self._bit_dict[name]
            else: 
                raise ValueError("{} isn't a defined bit".format(name))
        return bits
            
    @property
    def bits(self): 
        return self._get_bits(self.req_bits)
        
    @property
    def antibits(self):
        return self._get_bits(self.veto_bits)
    
    def get_config_dict(self): 
        """
        Produces the configuration info needed for _stacksusy
        """
        config_dict = {
            'jet_tag_requirements': self.cut_config['btag_config'], 
            'leading_jet_pt': self.cut_config['leading_jet_gev']*1e3, 
            'met': self.cut_config['met_gev']*1e3, 
            'required_bits': long(self.bits), 
            'veto_bits': long(self.antibits), 
            'type': self.type.upper(), 
            }
        return config_dict

# -- consider moving this ?
class Stacker(object): 
    """
    Constructed  with a regions dict. Runs the stacking routine when 
    hist_from_ntuple is called.
    """
    def __init__(self, regions_dict): 
        self._regions = {k:Region(v) for k,v in regions_dict.items()}
        self.dummy = False
    
    def hist_from_ntuple(self, ntuple, hist, systematic='NONE'): 
        # got to figure out how I'm going to deal with possibly 
        # differing configuration cuts... 
        # current plan, don't configure cuts, output a histogram and 
        # do the counting on that... we'll use stacksusy as a standin
        if isfile(hist): 
            raise IOError(5,"already exists",hist)
        regions = []
        for name, reg in self._regions.items(): 
            regdic = reg.get_config_dict()
            regdic['name'] = name
            regdic['output_name'] = hist 
            regdic['systematic'] = systematic
            regions.append(regdic)

        self._run(ntuple, regions)

    def run_multisys(self, ntuple, basedir, systematics, rerun=False): 
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
                    if rerun: 
                        os.remove(full_out_path)
                    else: 
                        continue
                regdic['output_name'] = full_out_path
                regdic['systematic'] = systematic
                regions.append(regdic)

        if regions: 
            self._run(ntuple, regions)

    def _run(self, ntuple, regions): 
        if self.dummy: 
            print ntuple
            for reg in regions: 
                print reg
            return 
        from stop.hyperstack import stacksusy
        flags = 'v'
        if basename(ntuple).startswith('d'): 
            flags += 'd'
        if not isfile(ntuple): 
            raise IOError(3,"doesn't exist",ntuple)
        stacksusy(ntuple, regions, flags=flags)
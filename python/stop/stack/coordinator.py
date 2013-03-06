import yaml
from stop import bits
from os.path import basename, isfile, isdir, join
import glob, os

class Coordinator(object): 
    """
    Constructed with a file-like object. Fills out the steering file, then
    can (re)run the d3pd -> hist (stacker) / aggregator routines. 

    This class should handle all the yaml parsing stuff. 

    General Design Principals: 
        - missing directories are generated when needed. 
        - missing 'files' entries are generated on initilization. 
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
                'hists': 'hists'}
    def __repr__(self): 
        return repr(self._config_dict)

    def stack(self, systematic='NONE', rerun=False): 
        ntup_dict = self._config_dict['files']['ntuples']
        if systematic in self.distiller_systematics: 
            ntuples = glob.glob('{}/*.root'.format(ntup_dict[systematic]))
        elif systematic in self.scale_factor_systematics: 
            ntuples = glob.glob('{}/*.root'.format(ntup_dict['NONE']))
        else: 
            raise ValueError('what the fuck is {}'.format(systematic))
        stacker = Stacker(self._config_dict['regions'])
        hists_path = self._config_dict['files']['hists']
        if not isdir(hists_path): 
            os.mkdir(hists_path)
        systdir_name = 'baseline' if systematic == 'NONE' else systematic
        syst_hist_path = join(hists_path, systdir_name.lower())
        if not isdir(syst_hist_path): 
            os.mkdir(syst_hist_path)

        for ntup in ntuples: 
            out_hist_name = '{}.h5'.format(splitext(basename(ntup))[0])
            out_hist_path = join(syst_hist_path, out_hist_name)
            print out_hist_path

    def check_regions(self): 
        for name, region_dict in self._config_dict['regions'].items(): 
            region = Region(region_dict)
            bits = region.bits
            antibits = region.antibits
            print '{}: {} {}'.format(name, bits, antibits)
            
    def write(self, yaml_file): 
        for line in yaml.dump(self._config_dict): 
            yaml_file.write(line)

class Region(object): 
    """
    Stores info on signal / control region. Bits are stored as strings 
    and used to look up real values. 
    """
    default_dict = { 
        'type':'control', 
        'req_bits':['preselection'], 
        'veto_bits':[], 
        'cut_config':{
            'leading_jet_gev':240, 
            'met_gev':180, 
            'btag_config':'LOOSE_TIGHT'
            }
        }
    _bit_dict = dict(bits.bits)
    _composite_bit_dict = dict(bits.composite_bits)
    _final_dict = bits.final_dict

    def __init__(self, yaml_dict={}): 
        if not yaml_dict: 
            yaml_dict = self.default_dict
        self.type = yaml_dict['type']
        self.req_bits = yaml_dict['req_bits']
        self.veto_bits = yaml_dict['veto_bits']
        self.cut_config = yaml_dict['cut_config']
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

class Stacker(object): 
    """
    Constructed  with a regions dict. Runs the stacking routine when 
    hist_from_ntuple is called.
    """
    def __init__(self, regions_dict): 
        self._regions = {k:Region(v) for k,v in regions_dict.items()}
    
    def hist_from_ntuple(self, ntuple, hist, systematic='NONE'): 
        # got to figure out how I'm going to deal with possibly 
        # differing configuration cuts... 
        # current plan, don't configure cuts, output a histogram and 
        # do the counting on that... we'll use stacksusy as a standin
        from stop.hyperstack import stacksusy
        first_config = self._regions.values()[0].cut_config
        if any(r.cut_config != first_config): 
            raise ValueError("mismatch in cut configurations")
        config_dict = dict(
            leading_jet = first_config['leading_jet_gev'] * 1e3, 
            met         = first_config['met_gev'] * 1e3, 
            btag_config = first_config['btag_config'], 
            systematic  = systematic)
        flags = 'v'
        if basename(ntuple).startswith('d'): 
            flags += 'd'
        if not isfile(ntuple): 
            raise IOError("{} doesn't exist".format(ntuple))
        if isfile(hist): 
            raise IOError("{} exists, refuse to overwrite".format(hist))
        bitslist = [
            (n, v.bits, v.antibits) for n, v in self._regions.items()]
        stacksusy(ntuple, bitslist, output_file=hist, flags=flags, 
                  config_opts=hack_config_dict)

import yaml
from stop import bits
from os.path import basename, isfile

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
    def __init__(self, yaml_file): 
        self._config_dict = yaml.load(yaml_file)
        if not 'regions' in self._config_dict: 
            example = Region().get_yaml_dict()
            self._config_dict['regions'] = {'example':example}
        
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
        'configuration':{
            'leading_jet':240e3, 
            'met':180e3, 
            'btag_config':'LOOSE_TIGHT'
            }
        }
    def __init__(self, yaml_dict={}): 
        if not yaml_dict: 
            yaml_dict = self.default_dict
        self.type = yaml_dict['type']
        self.req_bits = yaml_dict['req_bits']
        self.veto_bits = yaml_dict['veto_bits']
        self.cut_config = yaml_dict['configuration']
        self._bit_dict = dict(bits.bits)
        self._composite_bit_dict = dict(bits.composite_bits)
    def get_yaml_dict(self): 
        base = self.__dict__
        for k in base: 
            if k.startswith('_')
            del base[k]
        return base
            
    @property
    def bits(self): 
        bits = 0
        for name in self.req_bits: 
            if name in self._composite_bit_dict: 
                bits |= self._composite_bit_dict[name]
            elif name in self._bit_dict: 
                bits |= self._bit_dict[name]
        return bits
        
    @property
    def antibits(self)
        antibits = 0
        for name in self.veto_bits: 
            if name in self._composite_bit_dict: 
                antibits |= self._composite_bit_dict[name]
            elif name in self._bit_dict: 
                antibits |= self._bit_dict[name]
        return antibits

class Stacker(object): 
    """
    Constructed  with a regions dict. Runs the stacking routine when 
    hist_from_ntuple is called.
    """
    def __init__(self, regions_dict): 
        self._regions = {k:Region(v) for k,v in regions_dict}
    
    def hist_from_ntuple(self, ntuple, hist, systematic='NONE'): 
        # got to figure out how I'm going to deal with possibly 
        # differing configuration cuts... 
        # current plan, don't configure cuts, output a histogram and 
        # do the counting on that... we'll use stacksusy as a standin
        from stop.hyperstack import stacksusy
        hack_config_dict = dict(
            leading_jet = 240*GeV, 
            met         = 180*GeV, 
            btag_config = 'LOOSE_TIGHT', 
            systematic  = systematic)
        flags = ''
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

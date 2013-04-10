from stop import bits

class Region(object): 
    """
    Stores info on signal / control region. Bits are stored as strings 
    and used to look up real values. This class is also responsible for
    checking the integrity of its stored data. 
    """
    default_dict = { 
        'type':'control', 
        'bits': { 
            'required':['preselection'], 
            'veto':[], 
            'region_flags':[], 
            }, 
        'kinematics':{
            'leading_jet_gev':240, 
            'met_gev':180, 
            }, 
        'btag_config':['NOTAG','LOOSE','TIGHT'], 
        }
    _bit_dict = dict(bits.bits)
    _composite_bit_dict = dict(bits.composite_bits)
    _final_dict = bits.final_dict
    _allowed_types = set(['control','signal','validation'])
    _region_flags = bits.region_event_filter_bits

    def __init__(self, yaml_dict={}): 
        if not yaml_dict: 
            yaml_dict = self.default_dict
        self._read_dict(yaml_dict)

    def __repr__(self): 
        return repr(self.get_yaml_dict())

    def _read_dict(self,yaml_dict): 
        self.type = yaml_dict['type']
        self.bits = yaml_dict['bits']
        self.kinematics = yaml_dict['kinematics']
        self.btag_config = yaml_dict['btag_config']
        if self.type not in self._allowed_types: 
            raise ValueError('region type {} is not known'.format(
                    self.type))

    def get_yaml_dict(self): 
        """
        returns the object as a dict for yaml
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
        return long(bits)
            
    def get_bits(self): 
        return self._get_bits(self.bits['required'])
        
    def get_antibits(self):
        return self._get_bits(self.bits.get('veto',[]))
    
    def get_region_bits(self): 
        allbits = 0
        for name in self.bits.get('region_flags',[]):
            allbits |= self._region_flags[name]
        return long(allbits)
    
    def get_config_dict(self): 
        """
        Produces the configuration info needed for _stacksusy
        """
        config_dict = {
            'jet_tag_requirements': self.btag_config, 
            'leading_jet_pt': self.kinematics['leading_jet_gev']*1e3, 
            'met': self.kinematics['met_gev']*1e3, 
            'required_bits': self.get_bits(), 
            'veto_bits': self.get_antibits(), 
            'region_bits': self.get_region_bits(), 
            'type': self.type.upper(), 
            'hists': 'HISTMILL', 
            }
        return config_dict

class SuperRegionKey(object): 
    """
    Constructed from a Region, compares identical if the regions fall in the 
    same SuperRegion. 
    
    Not clear that this is any cleaner than a simple tuple... maybe 
    better to stick a superregion hash function in the region... 
    """
    def __init__(self, region): 
        self.req_bits = region.get_bits()
        self.veto_bits = region.get_antibits()
        self.region_bits = region.get_region_bits()  
        self.jet_tags = tuple(region.btag_config)
        self.region_type = region.type
    

class SuperRegion(object): 
    """
    Constructed from a list of regions. SuperRegions combine kinematics from 
    normal regions. 
    """
    kinematics = ['leading_jet_gev', 'met_gev']
    def __init__(self, met_lower_bound=0, leading_jet_lower_bound=120): 
        self.subregions = {}
        self.tuple = None
        self.kinematic_lower_bounds = {
            'leading_jet_gev': leading_jet_lower_bound, 
            'met_gev': met_lower_bound}
    def add_subregion(self, name, region):
        reg_tuple = _superregion_tuple(region)
        if self.tuple:
            if reg_tuple != self.tuple: 
                raise ValueError(
                    "tried to add an incompatible subregion"
                    " (adding {} to {})".format(reg_tuple, self.tuple))
        else: 
            self.tuple = reg_tuple
        for kin in self.kinematics: 
            if region.kinematics[kin] < self.kinematic_lower_bounds[kin]: 
                self.kinematic_lower_bounds[kin] = region.kinematics[kin]
        self.subregions[name] = region

    def get_common_name(self): 
        subnames = self.subregions.keys()
        if not subnames: 
            raise ValueError("no subregions in this superregion")
        min_len = min(len(x) for x in subnames)
        if not min_len: 
            return ''
        for cn in xrange(min_len): 
            if any(name[cn] != subnames[0][cn] for name in subnames): 
                return subnames[0][:cn]
        return subnames[0][:min_len]
    
    def get_yaml_dict(self): 
        subregions = {
            n:v.get_yaml_dict() for n,v in self.subregions.iteritems()}
        output = {
            'subregions':subregions, 
            'lower_bounds': self.kinematic_lower_bounds.copy(), 
            }
        return output
        
    def get_config_dict(self): 
        req, veto, jtags, reg_type, reg_bits = self.tuple
        lower_bounds = self.kinematic_lower_bounds
        config_dict = {
            'jet_tag_requirements': list(jtags), 
            'leading_jet_pt': lower_bounds['leading_jet_gev']*1e3, 
            'met': lower_bounds['met_gev']*1e3, 
            'required_bits': req, 
            'veto_bits': veto, 
            'region_bits': reg_bits, 
            'type': reg_type.upper(), 
            'hists': 'KINEMATIC_STAT', 
            }
        return config_dict
        

def condense_regions(regions): 
    """
    get superregions from normal regions
    """
    super_regions = {}
    for name, region in regions.iteritems(): 
        supertuple = _superregion_tuple(region)
        if not supertuple in super_regions: 
            super_regions[supertuple] = SuperRegion()
        super_regions[supertuple].add_subregion(name, region)
    used_names = {}
    named_regions = {}
    for region in super_regions.values(): 
        name = region.get_common_name()
        if name in used_names: 
            used_names[name] += 1
        else: 
            used_names[name] = 0
        final_name = name + str(used_names[name])
        named_regions[final_name] = region
    return named_regions

def _superregion_tuple(region): 
    req_bits = region.get_bits()
    veto_bits = region.get_antibits()
    region_bits = region.get_region_bits()
    jet_tags = tuple(region.btag_config)
    return (req_bits, veto_bits, jet_tags, region.type, region_bits)
        
    

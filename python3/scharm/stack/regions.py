

class Region(object): 
    """
    Stores info on signal / control region. Bits are stored as strings 
    and used to look up real values. This class is also responsible for
    checking the integrity of its stored data. 
    """
    default_dict = { 
        'selection': 'signal',
        'type':'signal', 
        'kinematics':{
            'leading_jet_gev':240, 
            'met_gev':180, 
            }, 
        'btag_config':[], 
        'tagger':'JFC', 
        'jet_tag_assignment': 'PT_ORDERED'
        }
    _allowed_types = set(['control','signal','validation'])

    def __init__(self, yaml_dict={}): 
        if not yaml_dict: 
            yaml_dict = self.default_dict
        self._read_dict(yaml_dict)

    def __repr__(self): 
        return repr(self.get_yaml_dict())

    def _read_dict(self,yaml_dict): 
        self.type = yaml_dict['type']
        self.kinematics = yaml_dict['kinematics']
        self.btag_config = yaml_dict['btag_config']
        if self.type not in self._allowed_types: 
            raise RegionConfigError('region type {} is not known'.format(
                    self.type))
        self.tagger = yaml_dict.get('tagger', None)
        self.hists = yaml_dict.get('hists', 'NMINUS')
        self.jet_tag_assignment = yaml_dict.get(
            'jet_tag_assignment','PT_ORDERED')
        self.boson_pt_correction = yaml_dict.get(
            'boson_pt_correction', 'MARKS')

    def get_yaml_dict(self): 
        """
        returns the object as a dict for yaml
        """
        # as long as the names don't change we can just dump the object data
        baselist = self.__dict__.items()
        base = {k:v for k, v in baselist if not k.startswith('_')}
        return base
    
    def get_config_dict(self): 
        """
        Produces the configuration info needed for _stacksusy
        """
        config_dict = {
            'selection': self.selection, 
            'jet_tag_requirements': self.btag_config, 
            'leading_jet_pt': self.kinematics['leading_jet_gev']*1e3, 
            'met': self.kinematics['met_gev']*1e3, 
            'type': self.type.upper(), 
            'hists': self.hists.upper(), 
            'tagger': _get_tagger(self.btag_config, self.tagger), 
            'jet_tag_assignment': self.jet_tag_assignment, 
            'boson_pt_correction': self.boson_pt_correction, 
            }
        return config_dict
    
class RegionConfigError(ValueError): 
    def __init__(self, message): 
        super(RegionConfigError,self).__init__(message)

def _get_tagger(jet_tags, tagger): 
    """
    Figure out the tagger based on the tags used. 
    """
    jfc_tags = {j for j in jet_tags if j.startswith('JFC')}
    non_jfc = set(jet_tags) - jfc_tags - set(['NOTAG'])
    if tagger:
        if jfc_tags or non_jfc: 
            raise RegionConfigError(
                "should only specify tagger when op are given")
        return tagger
    
    if jfc_tags and non_jfc: 
        raise RegionConfigError(
            "can't be mixing taggers (right now), tried to use {}".format(
                ', '.join(set(jet_tags))))
    if jfc_tags: 
        return 'JFC'
    else: 
        return 'CNN'

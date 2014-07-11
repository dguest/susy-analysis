
class Region:
    """
    Stores info on signal / control region. Bits are stored as strings
    and used to look up real values. This class is also responsible for
    checking the integrity of its stored data.
    """
    _allowed_types = set(['control','signal','validation'])

    def __init__(self, yaml_dict):
        self._read_dict(yaml_dict)

    def __repr__(self):
        return repr(self.get_yaml_dict())

    def _read_dict(self,yaml_dict):
        self.type = yaml_dict['type']
        self.selection = yaml_dict['selection']
        self.kinematics = yaml_dict['kinematics']
        self.stream = yaml_dict['stream']
        if self.type not in self._allowed_types:
            raise RegionConfigError('region type {} is not known'.format(
                    self.type))
        self.hists = yaml_dict.get('hists', 'NMINUS')
        self.boson_pt_correction = yaml_dict.get(
            'boson_pt_correction', 'NO_PT_CORRECTION')
        self.replacement = yaml_dict.get('replacement','normal')
        self.max_signal_jets = yaml_dict.get('max_signal_jets', 999)
        self.tagger = yaml_dict['tagger']

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
        Produces the configuration info needed for _stacksusy.
        Note that this lacks some important information, such as
        the 'output_name' and the 'stream'.
        """
        config_dict = {
            'selection': self.selection.upper(),
            'leading_jet_pt': self.kinematics['leading_jet_gev']*1e3,
            'second_jet_pt': self.kinematics.get('second_jet_gev', 50)*1e3,
            'first_lepton_pt': self.kinematics.get(
                'first_lepton_pt_gev', -1)*1e3,
            'met': self.kinematics['met_gev']*1e3,
            'mct': self.kinematics.get('mct_gev',-1)*1e3,
            'max_signal_jets': self.max_signal_jets,
            'type': self.type.upper(),
            'hists': self.hists.upper(),
            'boson_pt_correction': self.boson_pt_correction,
            'tagger': self.tagger.upper(),
            }
        return config_dict

# ___________________________________________________________________________
# sbottom definitions

_crz = 'CR_Z_1L'
_sbottom_cr = {'CR_W', 'CR_T', _crz}
def sbottom_regions():
    """
    return sbottom regions as a yml file
    """
    sbottom = {'signal': _sbottom_region('SIGNAL', 'jet')}
    for cr in _sbottom_cr:
        sbottom[cr.lower()] =  _sbottom_region(cr,'lepton')
    sbottom['preselection'] = _build_kinematic_region(
        'QUALITY_EVENT', 50, 150, stream='all')
    sbottom['preselection']['tagger'] = 'NONE'
    sbottom['met'] = _build_kinematic_region('VR_MET', lj=50, met=150)
    return sbottom

def _sbottom_region(version, stream):
    lj =  {'SIGNAL': 130, 'CR_W': 130, _crz: 50,  'CR_T': 130}[version]
    met = {'SIGNAL': 150, 'CR_W': 100, _crz: 100, 'CR_T': 100}[version]
    rpl = 'leptmet' if version == _crz else 'normal'
    return _build_kinematic_region(version, lj, met, rpl, stream)

def _build_kinematic_region(version, lj, met, rpl='normal', stream='jet'):
    control_regions = _sbottom_cr | {'QUALITY_EVENT'}
    default_dict = {
        'selection': version,
        'type': 'signal' if version not in control_regions else 'control',
        'kinematics':{
            'leading_jet_gev': lj,
            'met_gev': met,
            },
        'replacement': rpl,
        'stream': stream,
        'boson_pt_correction': 'MARKS',
        'tagger': 'JFC',
        }
    return default_dict

# ___________________________________________________________________________
# various helpers

class RegionConfigError(ValueError):
    def __init__(self, message):
        super(RegionConfigError,self).__init__(message)

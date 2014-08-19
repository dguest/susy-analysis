"""
Parsing tools to translate systematics textfile into the yaml input
"""

# __________________________________________________________________________
# define a few translation dicts

# translate will's proc names to mine
_proc_from_will = {'ttst':'top'}

# translate will's region names to mine
_reg_from_will = {'CR' + x.upper(): 'cr_' + x for x in 'wzt'}
_reg_from_will.update({'VRmCC':'vr_mcc', 'VRmCT':'vr_mct'})
for mct in [150, 200, 250]:
    _reg_from_will['mCT>{}'.format(mct)] = 'signal_mct{}'.format(mct)

# tie processes to control regions (for the extrapolation)
_proc_with_reg = [('Wjets', 'CRW'), ('Zjets','CRZ'), ('ttst','CRT')]

# __________________________________________________________________________
# other misc definitions

# various cleanup
def _shortreg(longreg):
    """this is to make will's naming scheme universal"""
    return longreg.replace('final','').replace('SRA','').replace(' ','')

class SystParser:
    """
    Read in systematics from file objects, stores them in .systs member
    """
    def __init__(self, systfile):
        """expects a file-like object"""

        self.records = set()

        systitr = iter(systfile)
        for line in systitr:
            if line.strip().endswith(':'):
                self._read_section(line, systitr)

    def dump(self):
        """print everything"""
        for syst in self.records:
            print(syst)

    def _read_section(self, line, systitr):
        method = line.rstrip(' :\n')
        fields = [x.strip() for x in next(systitr).split(',')]
        for line in systitr:

            # blank lines mean a new section is starting
            if not line.strip():
                return
            regvalsitr = iter(zip(fields, line.split(',')))
            process, *varlist = next(regvalsitr)[1].split('_',1)
            variation = '_'.join(varlist) if varlist else 'bare'
            for region, val in regvalsitr:
                syst = SysRecord(method, process, variation, region, val)
                self.records.add(syst)


class SysRecord:
    """class to hold one variant of a systematic"""
    def __init__(self, method, process, variation, region, value):
        self.method = method
        self.process = process
        self.variation = variation
        split_reg = [x.strip() for x in region.split('/')]
        self.region = _shortreg(split_reg[0])
        self.base_region = split_reg[1] if len(split_reg) > 1 else ''
        self.value = float(value)
    def _keyvals(self):
        """return `key` values, i.e. everything but self.value"""
        return (self.method, self.process, self.variation, self.region,
                self.base_region)
    def __str__(self):
        return ', '.join(list(self._keyvals()) + [str(self.value)])
    def __hash__(self):
        return hash(self._keyvals())
    def __eq__(self, other):
        return self._keyvals() == other._keyvals()


def get_standard_extrap(records):
    reg_from_proc = dict(_proc_with_reg)
    for rec in records:
        # Don't extrapolate to ourselves (this is nonzero in Will's
        # files because of statistical error?)
        if rec.region == rec.base_region:
            continue
        if rec.base_region == reg_from_proc.get(rec.process):
            yield rec

def combine_overalls(records, translate=True):
    """takes SysRecords, returns {(process, region):[down, up],...}"""
    ups = {}
    dns = {}
    for rec in records:
        if rec.variation.endswith('DOWNs_stat'):
            dns[rec.process, rec.region] = rec.value
        elif rec.variation.endswith('UPs_stat'):
            ups[rec.process, rec.region] = rec.value

    allkey = set(ups) | set(dns)
    if not len(allkey) == len(ups) == len(dns):
        raise ValueError('bad records')
    outdic = {}
    ptran, rtran = {}, {}
    if translate:
        ptran, rtran = (_proc_from_will, _reg_from_will)
    for keytup in allkey:
        proc, reg = keytup
        newkey = ptran.get(proc,proc), rtran.get(reg,reg)
        outdic[newkey] = [1 - dns[keytup], 1 + ups[keytup]]
    return outdic

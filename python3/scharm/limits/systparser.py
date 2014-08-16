"""
Parsing tools to translate systematics textfile into the yaml input
"""

class SysRecord:
    """class to hold one variant of a systematic"""
    def __init__(self, method, process, variation, region, value):
        self.method = method
        self.process = process
        self.variation = variation
        self.region = region
        self.value = float(value)
    def __str__(self):
        nsort = sorted(list(vars(self).items()))
        return ', '.join(str(x[1]) for x in nsort)


class SystParser:
    """Read in systematics from file objects, write them as dicts"""
    def __init__(self, systfile):
        """expects a file-like object"""

        self._systs = set()

        systitr = iter(systfile)
        for line in systitr:
            if line.strip().endswith(':'):
                self._read_section(line, systitr)

    def dump(self):
        """print everything"""
        for syst in self._systs:
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
            variation = varlist[0] if varlist else 'bare'
            for region, val in regvalsitr:
                syst = SysRecord(method, process, variation, region, val)
                self._systs.add(syst)



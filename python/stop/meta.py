
class Dataset(object): 
    """
    container for dataset info
    """
    def __init__(self): 
        self.id = 0
        self.name = ''
        self.xsec = -1
        self.kfactor = -1
        self.filteff = -1
        self.files = []
        self.evts = 0
    def corrected_xsec(self): 
        assert all(x >= 0 for x in [self.xsec, self.kfactor, self.filteff])
        return self.xsec * self.kfactor * self.filteff
    @property
    def xsec_per_evt(self): 
        """
        corrected cross section / number of events
        """
        return self.corrected_xsec() / self.evts

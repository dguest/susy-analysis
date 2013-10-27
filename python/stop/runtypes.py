import copy
class DsRange(object): 
    def __init__(self, *range_tuples): 
        self.range_tuples = range_tuples
        self.ds_list = []
    def __add__(self, other): 
        all_tuples = list(self.range_tuples)  
        ds_list = copy.deepcopy(self.ds_list)
        if isinstance(other,list): 
            ds_list += other
        else: 
            all_tuples += list(other.range_tuples)

        new = DsRange(*all_tuples)
        new.ds_list = ds_list
        return new
    def __radd__(self, other): 
        return self.__add__(other)
    def __iter__(self): 
        for rt in self.range_tuples: 
            low, high = rt
            for v in xrange(low, high+1): 
                yield v
        for v in self.ds_list: 
            yield v

marks_types = { 
    'ttbar': [117050], 
    'Wjets': DsRange( 
        (167740,167748), 
        (167761,167796), 
        (180534,180542) ), 
    'Zjets': DsRange( 
        (167749,167760), 
        (167797,167844), 
        (180543,180551) ), 
    't': DsRange( (110140,110143) ) + [110101,110119], 
    'diboson': [143065] + DsRange( (157814,157819)), 
    }

stop_signal = {
    'signal': DsRange( (157851, 157908) , (176270, 176320) )
}

scharm = {
    # 'squark-signal': DsRange( (162000,162349)), 
    'scharm-signal': DsRange( (183424,183474)), 
    }

variations = { 
    # truth level... maybe not for now
    # 'ttbar_scale': DsRange( (174763, 174766)), 
    'ttbar_parton_shower_jimmy': [105860], 
    'ttbar_parton_shower_pythia': [105861], 
    'ttbar_isr_fsr_more': [117209], 
    'ttbar_isr_fsr_less': [117210], 
    'ttbar_mcnlo': [105200], 
    # 'ttbar_alpgen': DsRange((164440,164443), (164450, 164453)) + [
    #     116108, 116109], 
    'ttbar_sherpa': DsRange((117800, 117809)), 
    # 'ttbar_powheg_pythia':[117050], 
    }
    

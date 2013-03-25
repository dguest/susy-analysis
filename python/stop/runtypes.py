class DsRange(object): 
    def __init__(self, *range_tuples): 
        self.range_tuples = range_tuples
    def __add__(self, other): 
        all_tuples = list(self.range_tuples)  
        if isinstance(other,list): 
            for v in other: 
                all_tuples.append( (v,v))
        else: 
            all_tuples += list(other.range_tuples)

        return DsRange(*all_tuples)
    def __radd__(self, other): 
        return self.__add__(other)
    def __iter__(self): 
        for rt in self.range_tuples: 
            low, high = rt
            for v in xrange(low, high+1): 
                yield v

marks_types = { 
    'ttbar': [105861], 
    'Wjets': DsRange( (167740,167748), (167761,167796)), 
    'Zjets': DsRange( (167749,167760), (167797,167844)), 
    't': DsRange( (108343,108346), (117360,117362)), 
    'diboson': [143065] + DsRange( (157814,157819)), 
    'signal': DsRange( (157851, 157908) , (176270, 176320) )
    }

        

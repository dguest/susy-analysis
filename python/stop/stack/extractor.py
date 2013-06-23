from stop.hists import HistNd
from h5py import File
from os.path import isfile
from stop.stack.regions import Region, superregion_tuple
from itertools import product

class RegionExtractor(object): 
    """
    Class is built from superregion defs, extracts region counts. 
    """
    cut_tolerance = 0.0001
    count_variables = ['kinematics','kinematicStats']
    def __init__(self, superregions): 
        self.superregions = superregions

    def _checkcut(self, target, actual): 
        reldiff = abs( (target - actual) / (target + actual))
        if reldiff > self.cut_tolerance: 
            raise ExtractionError(
                "relative difference between target and actual cut: {}, "
                "limited to {}".format(reldiff, self.cut_tolerance))

    def _get_subregion_counts(self, region, normed): 
        for axis in normed.axes.values(): 
            if not axis.type == 'integral': 
                axis.integrate(reverse=True, in_place=True)
        kin = region['kinematics']
        met_cut = kin['met_gev']*1e3
        lead_cut = kin['leading_jet_gev']*1e3

        lslice, lext = normed.axes['leadingJetPt'].get_slice(lead_cut*1.0001)
        self._checkcut(lead_cut, lext[0])
        mslice, mext = lslice.axes['met'].get_slice(met_cut*1.0001)
        self._checkcut(met_cut, mext[0])
        return mslice

    def extract_counts_file(self, h5_file_name): 
        """
        Extract subregions from a file. 

        Expects the (maybe questionable) standard input: 
        <physics-type>/{kinematics,kinematicStats}/<region>
        
        The returned dict is keyed by (physics, variable, region)
        """
        if not isfile(h5_file_name): 
            raise IOError("{} doesn't exist".format(h5_file_name))
        extracted = {}
        with File(h5_file_name) as h5: 
            for phname, methgroup in h5.iteritems(): 
                for meth, reggroup in methgroup.iteritems(): 
                    for sname, superregion in reggroup.iteritems(): 
                        normed = HistNd(superregion)
                        subregions = self.superregions[sname]['subregions']
                        for name, region in subregions.iteritems(): 
                            counts = self._get_subregion_counts(
                                region, normed)
                            extracted[phname,meth,name] = counts
        return extracted
    
    def extract_counts_dict(self, superdict, reg_dicts, 
                            variable='kinematics'): 
        """
        Extract subregions from a (physics, variable, superegion) keyed dict. 
        Returns a (physics, region) tuple. 
        """
        all_varaibles = set(k[1] for k in superdict)
        if not variable in all_varaibles: 
            raise ValueError(
                "can't extract {}, allowed variables: {}".format(
                    variable, ', '.join(all_varaibles)))
        super_regions = {}
        all_phys = set()
        for (phys, var, supname), hist in superdict.iteritems(): 
            if var != variable: 
                continue
            subregions = self.superregions[supname]['subregions']
            tup = None
            for regname, region_dict in subregions.iteritems(): 
                region = Region(region_dict)
                if not tup: 
                    tup = superregion_tuple(region)
                elif tup != superregion_tuple(region): 
                    raise ValueError(
                        'inconpatible subregions found'
                        ' in superregion {}'.format(supname))
            super_regions[tup] = supname
            all_phys.add(phys)
        
        regions = {}
        for phys, regname in product(all_phys, reg_dicts.keys()): 
            region = Region(reg_dicts[regname])
            regtup = superregion_tuple(region)
            super_region = super_regions[regtup]
            if not (phys, variable, super_region) in superdict: 
                continue
            hist = superdict[phys, variable, super_region]
            count = self._get_subregion_counts(reg_dicts[regname], hist)
            regions[phys,regname] = count
        return regions
            

class ExtractionError(StandardError): 
    def __init__(self, problem): 
        super(ExtractionException,self).__init__(problem)
        

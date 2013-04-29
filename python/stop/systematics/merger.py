class RegionMerger(object): 
    """
    Generates merged regions from a region counts dict. 
    """
    def __init__(self, counts): 
        self.counts = counts

    def _sum_regions(self, region_dict, merged_regions): 
        if isinstance(region_dict[merged_regions[0]],dict): 
            sum_normed = 0.0
            sum_wt2 = 0.0
            for reg_name in merged_regions: 
                region_entry = region_dict[reg_name]
                normed = region_entry['normed']
                n_eff = region_entry['stats']
                if n_eff: 
                    sum_wt2 += normed**2 / n_eff
                    sum_normed += normed
            if sum_wt2: 
                eff_stats = sum_normed**2 / sum_wt2
            else: 
                eff_stats = 0.0
            return {'normed': sum_normed, 'stats': eff_stats}
        else: 
            sum_normed = 0.0
            for reg_name in merged_regions: 
                sum_normed += region_dict[reg_name]
            return sum_normed
                

    def merge(self, merged_regions, new_region): 
        for sys_name, phys_dict in self.counts.iteritems(): 
            for phys_name, region_dict in phys_dict.iteritems():
                region = self._sum_regions(region_dict, merged_regions)
                self.counts[sys_name][phys_name][new_region] = region

    def merge_via_steering(self, steering_file):
        merged_regions = {}
        for region_name, region in steering_file['regions'].iteritems(): 
            merged_region = region.get('element_of')
            if merged_region: 
                l = merged_regions.setdefault(merged_region,[])
                l.append(region_name)

        for new_region, constituents in merged_regions.iteritems(): 
            try: 
                self.merge(constituents, new_region)
            except TypeError as err: 
                raise RegionMergeError(
                    '{}, trying to merge: {} in {}'.format(
                        err, ', '.join(constituents), new_region))

class RegionMergeError(StandardError): 
    def __init__(self, args): 
        super(RegionMergeError,self).__init__(args)
        

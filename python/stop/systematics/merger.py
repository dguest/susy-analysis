class RegionMerger(object): 
    """
    Generates merged regions from a region counts dict. 
    """
    def __init__(self, counts): 
        self.counts = counts

    def merge(self, merged_regions, new_region): 
        for sys_name, phys_dict in self.counts.iteritems(): 
            for phys_name, region_dict in phys_dict.iteritems(): 
                self.counts[sys_name][phys_name][new_region] = sum(
                    region_dict[x] for x in merged_regions)

    def merge_via_steering(self, steering_file):
        merged_regions = {}
        for region_name, region in steering_file['regions'].iteritems(): 
            merged_region = region.get('element_of')
            if merged_region: 
                l = merged_regions.setdefault(merged_region,[])
                l.append(region_name)

        for new_region, constituents in merged_regions.iteritems(): 
            self.merge(constituents, new_region)

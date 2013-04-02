import re

class TransferFactor(object): 
    """
    Basic transfer factor info: value, relative error, type fraction
    in signal region. 
    """
    def __init__(self, value, error): 
        self.value = value
        self.error = error
        self.physics = None
        self.sr_frac = None
        self.sr_frac_err = None
        self.mc_only = None
        self.mc_only_err = None
        self.transfer_rel_errors = {}

class TransferTable(object): 
    """
    Uses the Transfer Calculator and the stacker steering file to generate
    transfer factors from each of the control / signal combinations. 
    """
    def __init__(self, stack_steering, counts): 
        """
        Inputs are expected to be dicts. 
        """
        counted_regions = set(next(counts['NONE'].iteritems())[1].keys())
        regions = stack_steering['regions']
        self.control_regions = set()
        self.signal_regions = set()
        for region_name, region in regions.iteritems(): 
            name = region_name
            father_region = region.get('element_of')
            if father_region and father_region in counted_regions: 
                name = father_region
            if region['type'] == 'control': 
                self.control_regions.add(name)
            elif region['type'] == 'signal': 
                self.signal_regions.add(name)
        self.counts = counts
    def get_tf_table(self, physics_type): 
        """
        Returns nested dicts. 
        Hierarchy: 
            - Signal Region
            - Control Region
            - TF / error tuple
        """
        calc = TransferCalculator(self.counts, physics_type)
        signal_regions = {}
        for cr in self.control_regions: 
            signal_regions[cr] = {}
            for sr in self.signal_regions: 
                factor, err = calc.get_tf_and_err(cr, sr)
                tf = TransferFactor(factor,err)
                tf.physics = physics_type
                tf.sr_frac, tf.sr_frac_err = (
                    calc.get_controlled_fraction(sr))
                tf.mc_only, tf.mc_only_err = calc.get_total_mc(sr)
                tf.transfer_rel_errors = calc.get_coor_tf_rel_variations(
                    cr, sr)
                signal_regions[cr][sr] = tf

        return signal_regions

class TransferCalculator(object): 
    """
    Assumes the following structure for the counts passed in: 
      - systematic
      - physics type
      - region 

    The physics type can be changed after initialization, however. 

    TODO: centralize the lists of systematics.
    """

    def __init__(self, counts, physics_type='ttbar'): 
        self.counts = counts
        self.physics_type = physics_type
        sysvars = ['JES'] + list('BCUT')
        self.variations = ['JER'] + [
            sys + var for sys in sysvars for var in ['UP','DOWN']]
        self.baseline = 'NONE'
        self.data = 'data'
        signal_re = re.compile('stop-([0-9]+)-([0-9]+)')
        alltypes = self.counts[self.baseline].keys()
        self.signals = [s for s in alltypes if signal_re.search(s)]
        self.stat_factor = 1.0

    def _bare_tf(self, control, signal, variation=None): 
        if not variation: 
            variation = self.baseline
        control_count = self.counts[variation][self.physics_type][control]
        signal_count = self.counts[variation][self.physics_type][signal]
        if control_count == 0.0: 
            return float('nan')
        return signal_count / control_count
    
    def _get_statvar(self, region, physics, variation): 
        basestat = self.counts[self.baseline][physics][region]
        var = basestat**0.5 
        if variation == 'STAT': 
            return basestat + var
        else: 
            return basestat

    def _get_other_mc_in_region(self, control, variation=None): 
        if not variation: 
            variation = self.baseline
        excluded_mc = [self.data, self.physics_type] + self.signals
        other_mc = []
        for m in self.counts[self.baseline]: 
            if m not in excluded_mc: 
                other_mc.append(m)
        return sum(self.counts[variation][m][control] for m in other_mc)

    def _predicted_count_phystype(self, control, signal, variation=None): 
        if not variation: 
            variation = self.baseline
        dc = self._get_statvar(control, self.data, variation)
        if variation.startswith('STAT'): 
            variation = self.baseline
        tf = self._bare_tf(control, signal, variation)
        excluded_mc = [self.data, self.physics_type] + self.signals
        other_mc = []
        for m in self.counts[self.baseline]: 
            if m not in excluded_mc: 
                other_mc.append(m)
        oc_sim = self._get_other_mc_in_region(control)
        prediction = tf * (dc - oc_sim)
        return prediction

    def get_mc_uncertainties(self, signal, physics_type): 
        signal_count = self.counts[self.baseline][physics_type][signal]
        variations = {}
        for variation in self.variations: 
            var_count = self.counts[variation][physics_type][signal]
            variations[variation] = var_count - signal_count
        return variations

    def get_mc_uncertainty(self, signal, physics_type): 
        variations = self.get_mc_uncertainties(signal, physics_type).values()
        total_error = sum( (v**2 for v in variations))**(0.5)
        return total_error

    def get_total_mc(self, signal, exclude_list=[]): 
        all_mc_types = set(self.counts[self.baseline].keys()) 
        all_mc_types -= set([self.data] + self.signals + exclude_list)
        baseline = self.counts[self.baseline]
        total_mc = sum(baseline[t][signal] for t in all_mc_types)
        uncertainties = []
        for phys_type in all_mc_types: 
            uncertainties.append(self.get_mc_uncertainty(signal, phys_type))
        return total_mc, sum( (v**2 for v in uncertainties))**(0.5)

    def predict_count_phystype_variations(self, control, signal): 
        all_counts = {}
        add_variations = ['STAT', self.baseline]
        for variation in self.variations + add_variations:
            predicted = self._predicted_count_phystype(
                control, signal, variation)
            all_counts[variation] = predicted
        return all_counts

    def get_coor_tf_rel_variations(self, control, signal): 
        predicted = self.predict_count_phystype_variations(control, signal)
        baseline_pred = predicted[self.baseline]
        stat_keys = [k for k in predicted if k.startswith('STAT')]
        nonsyst_keys = [self.baseline] + stat_keys
        syst_pred = { 
            k:p for k,p in predicted.iteritems() if k not in nonsyst_keys}

        base_count = (
            self.counts[self.baseline][self.data][control] - 
            self._get_other_mc_in_region(control) )
        if base_count == 0.0: 
            base_count = float('nan')
        base_tf = baseline_pred / base_count
        syst_tf = {
            k:syst_pred[k] / base_count for k in syst_pred}
        def get_rel(var): 
            return (var - base_tf) / base_tf
        rel_var = {k:get_rel(sys) for k, sys in syst_tf.iteritems()}

        if baseline_pred <= 0.0: 
            stat_rel_error = float('nan')
        else: 
            stat_rel_error = (
                predicted['STAT'] - baseline_pred) / baseline_pred

        rel_var['STAT'] = stat_rel_error
        return rel_var

    def get_tf_and_err(self, control, signal): 
        base_tf = self._bare_tf(control, signal)
        rel_variations = self.get_coor_tf_rel_variations(control, signal)
        sum_rel_var = sum(v**2 for v in rel_variations.values())**0.5
        sum_variations = sum_rel_var * base_tf
        return base_tf, sum_variations

    def predict_count_coor_errors(self, control, signal): 
        all_counts = self.predict_count_variations(control, signal)
        base = all_counts[self.baseline]
        variations = [v for k,v in all_counts.items() if k != self.baseline]
        errors = [base - c for c in variations]
        sum_square_errors = sum(e**2 for e in errors)
        return base, sum_square_errors**(0.5)

    def get_controlled_fraction(self, signal): 
        baseline = self.counts[self.baseline]
        exc_mc, exc_err = self.get_total_mc(signal,[self.physics_type])
        this_mc = baseline[self.physics_type][signal]
        this_err = self.get_mc_uncertainty(signal, self.physics_type)
        total = exc_mc + this_mc
        
        this_err_factor = (1.0/total - this_mc / total**2) 
        exc_err_factor = this_mc/total**2
        final_err = this_err_factor * this_err + exc_err_factor * exc_err
        
        return this_mc/total, final_err

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

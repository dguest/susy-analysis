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
        self.transfered = None
        self.transfered_err = None
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
        regions = stack_steering['regions']
        self.control_regions = {}
        self.signal_regions = {}
        for region_name, region in regions.iteritems(): 
            name = region_name
            if region['type'] == 'control': 
                self.control_regions[name] = region
            elif region['type'] == 'signal': 
                self.signal_regions[name] = region 
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
                factor, err = calc.get_transfer_factor(cr, sr)
                tf = TransferFactor(factor,err)
                tf.physics = physics_type
                tf.sr_frac, tf.sr_frac_err = (
                    calc.get_controlled_fraction(sr))
                tf.transfered, tf.transfered_err = (
                    calc.predict_count_coor_errors(cr,sr))
                tf.mc_only, tf.mc_only_err = calc.get_total_mc(sr)
                tf.transfer_rel_errors = calc.get_transfer_rel_errors(
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
        self.variations = [
            sys + var for sys in sysvars for var in ['UP','DOWN']]
        self.baseline = 'NONE'
        self.data = 'data'
        signal_re = re.compile('stop-([0-9]+)-([0-9]+)')
        alltypes = self.counts[self.baseline].keys()
        self.signals = [s for s in alltypes if signal_re.search(s)]
        self.stat_factor = 1.0
    
    def _get_rel_error(self, variation, control): 
        control_count = self.counts[self.baseline][self.physics_type][control]
        control_var = self.counts[variation][self.physics_type][control]
        if control_count == 0.0: 
            return float('nan')
        return control_var / control_count
        
    def _get_rel_error_diff(self, variation, control, signal): 
        control_rel = self._get_rel_error(variation, control)
        signal_rel = self._get_rel_error(variation, signal)
        return signal_rel - control_rel

    def _stat_err(self, number): 
        return (number * self.stat_factor)**(0.5)

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
        # variation is halved because there are two... correct? 
        var = (basestat / 2)**0.5 
        if variation == 'STATUP': 
            return basestat + var
        elif variation == 'STATDOWN': 
            return basestat - var
        else: 
            return basestat

    def _predicted_count(self, control, signal, variation=None): 
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
        oc_sim = sum(self.counts[variation][m][control] for m in other_mc)
        os_sim = sum(self.counts[variation][m][signal] for m in other_mc)
        prediction = tf * (dc - oc_sim) + os_sim
        return prediction

    def get_transfer_rel_errors(self, control, signal): 
        rel_errors = {}
        for variation in self.variations: 
            rel_var = self._get_rel_error_diff(variation, control, signal)
            rel_errors[variation] = rel_var

        control_data = self.counts[self.baseline][self.data][control]
        data_stat_err = self._stat_err(control_data)
        control_mc, control_exc_mc_err = self.get_total_mc(
            control, [self.physics_type])
        subtr_control = control_data - control_mc

        if subtr_control == 0.0: 
            subtr_control = float('nan')
        subtr_control_rel_err = (
            data_stat_err**2 + control_exc_mc_err**2)**(0.5) / subtr_control
        
        rel_errors['STAT'] = subtr_control_rel_err

        return rel_errors

    def get_transfer_factor(self, control, signal, errors='all'): 
        base_factor = self._bare_tf(control, signal)

        rel_errors = self.get_transfer_rel_errors(control, signal)
        if errors == 'all': 
            errors = [v for k,v in rel_errors.items() if k in errors]
        else: 
            errors = rel_errors.values()
        total_rel_error = sum( (v**2 for v in rel_errors.values()))**(0.5)
        total_error = base_factor * total_rel_error
        return base_factor, total_error

    def get_mc_uncertainties(self, signal, physics_type): 
        signal_count = self.counts[self.baseline][physics_type][signal]
        variations = {}
        for variation in self.variations: 
            var_count = self.counts[variation][physics_type][signal]
            variations[variation] = var_count - signal_count
        return variations
    
    def get_mc_rel_errors(self, signal, physics_type): 
        variations = self.get_mc_uncertainties(signal, physics_type)
        nominal = self.counts[self.baseline][physics_type][signal]
        rel_errors = {}
        for key, var in variations.items(): 
            rel_error = var / nominal
            rel_errors[key] = rel_error
        return rel_errors

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

    def predict_counts(self, control, signal, add_non_tf_error=True): 
        baseline = self.counts[self.baseline]
        control_data = baseline[self.data][control]
        transfer_factor, tf_error = self.get_transfer_factor(control, signal)
        control_exc_mc, control_exc_mc_err = self.get_total_mc(
            control, [self.physics_type])
        signal_exc_mc, signal_exc_mc_err = self.get_total_mc(
            signal, [self.physics_type])


        # the mc-subtracted control region 
        subtr_control = control_data - control_exc_mc

        transfered = transfer_factor * subtr_control

        # expected is tf * mc-subtracted control + signal region mc
        exp_data = transfered + signal_exc_mc

        # --- calculate errors ----
        # transfered error (without final error for simulation in 
        # signal region)
        transfered_err = tf_error * transfered

        # total error
        total_error = (transfered_err**2 + signal_exc_mc_err**2)**(0.5)

        return exp_data, total_error

    def predict_count_variations(self, control, signal): 
        all_counts = {}
        add_variations = ['STATUP','STATDOWN', self.baseline]
        for variation in self.variations + add_variations:
            all_counts[variation] = self._predicted_count(
                control, signal, variation)
        return all_counts

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

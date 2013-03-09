import re
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
        return control_var / control_count
        
    def _get_rel_error_diff(self, variation, control, signal): 
        control_rel = self._get_rel_error(variation, control)
        signal_rel = self._get_rel_error(variation, signal)
        return signal_rel - control_rel

    def _stat_err(self, number): 
        return (number * self.stat_factor)**(0.5)

    def get_transfer_rel_errors(self, control, signal): 
        rel_errors = {}
        for variation in self.variations: 
            rel_var = self._get_rel_error_diff(variation, control, signal)
            rel_errors[variation] = rel_var
        return rel_errors

    def get_transfer_factor(self, control, signal): 
        control_count = self.counts[self.baseline][self.physics_type][control]
        signal_count = self.counts[self.baseline][self.physics_type][signal]
        base_factor = signal_count / control_count

        rel_errors = self.get_transfer_rel_errors(control, signal)
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
        tf_rel_error = tf_error / transfer_factor
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
        data_stat_err = self._stat_err(control_data)
        subtr_control_rel_err = (
            data_stat_err**2 + control_exc_mc_err**2)**(0.5) / subtr_control

        # transfered error (without final error for simulation in 
        # signal region)
        transfered_rel_err = (
            tf_rel_error**2 + subtr_control_rel_err**2)**(0.5) 
        transfered_err = transfered_rel_err * transfered

        # total error
        total_error = (transfered_err**2 + signal_exc_mc_err**2)**(0.5)

        return exp_data, total_error

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

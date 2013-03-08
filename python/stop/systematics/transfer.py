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

    def _get_rel_error(self, variation, control): 
        control_count = self.counts[self.baseline][self.physics_type][control]
        control_var = self.counts[variation][self.physics_type][control]
        return control_var / control_count
        
    def _get_rel_error_diff(self, variation, control, signal): 
        control_rel = self._get_rel_error(variation, control)
        signal_rel = self._get_rel_error(variation, signal)
        return signal_rel - control_rel

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

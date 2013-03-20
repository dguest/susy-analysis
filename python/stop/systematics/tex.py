
class TransferTexPrinter(object): 
    """
    Expects to be initialized by a dict with hierarchy: 
        - Control Region 
        - Signal Region
        - Transfer Factor object
    """
    def __init__(self, tf_table): 
        self.tf_table = tf_table
        control_regions = set(tf_table.keys())
        signal_regions = set()
        for cr in control_regions: 
            signal_regions |= set(tf_table[cr].keys())
        
        self.control_regions = sorted(control_regions)
        self.signal_regions = sorted(signal_regions)

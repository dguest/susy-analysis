import matplotlib.pyplot as plt
from matplotlib.ticker import LogFormatter, LogLocator
from matplotlib.colors import LogNorm, Normalize
import matplotlib as mpl
import numpy as np
import yaml

class TransferFactorPlot(object): 
    """
    Expects to be initialized with transfer factor dictionary. 
    Hierarchy: 
        - Signal Region
        - Control Region
        - TF / error tuple
    """
    def __init__(self, transfer_factors): 
        self.transfer_factors = transfer_factors
        self._setup_positions()
        self.dotsize = 120
    def _setup_positions(self): 
        signal_regions = set(self.transfer_factors.keys())
        control_regions = set()
        for sr in signal_regions: 
            control_regions |= set(self.transfer_factors[sr].keys())
        self.signal_regions = sorted(list(signal_regions))
        self.control_regions = sorted(list(control_regions))
        
        self.sr_pos = {k:p for p,k in enumerate(self.signal_regions)}
        self.cr_pos = {k:p for p,k in enumerate(self.control_regions)}

    def save_plot(self, name): 
        x = []                  # control regions
        y = []                  # signal regions
        z = []                  # something? 
        for srname, crdict in self.transfer_factors.iteritems(): 
            for crname, tf in crdict.iteritems(): 
                x.append(self.cr_pos[crname])
                y.append(self.sr_pos[srname])
                z.append(tf.error / tf.value)

        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        sc = ax.scatter(x, y, c=z, s=self.dotsize)
        ax.set_xticks([self.cr_pos[x] for x in self.control_regions])
        ax.set_xticklabels(self.control_regions)
        ax.set_yticks([self.sr_pos[x] for x in self.signal_regions])
        ax.set_yticklabels(self.signal_regions)
        cb = plt.colorbar(sc)
        fig.savefig(name, bbox_inches='tight')

import matplotlib.pyplot as plt
from matplotlib.ticker import LogFormatter, LogLocator
from matplotlib.colors import LogNorm, Normalize
import matplotlib as mpl
import numpy as np
import yaml
from stop.style import texify_sr

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
        control_regions = set(self.transfer_factors.keys())
        signal_regions = set()
        def cr_sort(key): 
            splkey = key.split('_')
            if len(splkey) == 1: 
                return '1' + key
            return ''.join(splkey[::-1])

        for cr in control_regions: 
            signal_regions |= set(self.transfer_factors[cr].keys())
        self.signal_regions = sorted(list(signal_regions))
        self.control_regions = sorted(list(control_regions), key=cr_sort)
        
        self.sr_pos = {k:p for p,k in enumerate(self.signal_regions)}
        self.cr_pos = {k:p for p,k in enumerate(self.control_regions)}

    def save_plot(self, name): 
        x = []                  # control regions
        y = []                  # signal regions
        z = []                  # something? 
        physicises = set()
        for crname, srdict in self.transfer_factors.iteritems(): 
            for srname, tf in srdict.iteritems(): 
                y.append(self.cr_pos[crname])
                x.append(self.sr_pos[srname])
                z.append(tf.error / tf.value)
                physicises.add(tf.physics)

        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        sc = ax.scatter(x, y, c=z, s=self.dotsize)
        ax.set_xticks([self.sr_pos[x] for x in self.signal_regions])
        ax.set_xticklabels([texify_sr(x) for x in self.signal_regions])
        ax.set_yticks([self.cr_pos[x] for x in self.control_regions])
        ax.set_yticklabels([texify_sr(x) for x in self.control_regions])
        cb = plt.colorbar(sc)
        if not len(physicises) == 1: 
            raise IOError('too many physicises')
        physics_type = next(iter(physicises))
        lab = '{} relative transfer factor error'.format(physics_type)
        cb.set_label(lab, y=0.98, va='top')
        fig.savefig(name, bbox_inches='tight')

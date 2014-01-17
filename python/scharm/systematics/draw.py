import matplotlib.pyplot as plt
from matplotlib.ticker import LogFormatter, LogLocator, LogFormatterMathtext
from matplotlib.colors import LogNorm, Normalize
import matplotlib as mpl
import numpy as np
import yaml
from stop.style import texify_sr, cr_sort
from os.path import splitext, basename

class TransferFactorPlot(object): 
    """
    Expects to be initialized with transfer factor dictionary. 
    Hierarchy: 
        - Control Region
        - Signal Region
        - TF / error tuple
    """
    def __init__(self, transfer_factors): 
        self.transfer_factors = transfer_factors
        self._setup_positions()
        self.dotsize = 120
    def _setup_positions(self): 
        control_regions = set(self.transfer_factors.keys())
        signal_regions = set()

        for cr in control_regions: 
            signal_regions |= set(self.transfer_factors[cr].keys())
        self.signal_regions = sorted(list(signal_regions))
        self.control_regions = sorted(list(control_regions), key=cr_sort)
        
        self.sr_pos = {k:p for p,k in enumerate(self.signal_regions)}
        self.cr_pos = {k:p for p,k in enumerate(self.control_regions)}

    def save_tf_plot(self, name): 
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

    def save_tf_rel_improvement_plot(self, name): 
        x = []                  # control regions
        y = []                  # signal regions
        z = []                  # something? 
        physicises = set()
        for crname, srdict in self.transfer_factors.iteritems(): 
            for srname, tf in srdict.iteritems(): 
                y.append(self.cr_pos[crname])
                x.append(self.sr_pos[srname])
                tf_rel_error = tf.transfered_err / tf.transfered
                mc_rel_error = tf.mc_only_err / tf.mc_only
                z.append(tf_rel_error - mc_rel_error)
                physicises.add(tf.physics)

        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        sc = ax.scatter(x, y, c=z, s=self.dotsize, vmin=-0.015, vmax=0.015)
        ax.set_xticks([self.sr_pos[x] for x in self.signal_regions])
        ax.set_xticklabels([texify_sr(x) for x in self.signal_regions])
        ax.set_yticks([self.cr_pos[x] for x in self.control_regions])
        ax.set_yticklabels([texify_sr(x) for x in self.control_regions])
        cb = plt.colorbar(sc)
        if not len(physicises) == 1: 
            raise IOError('too many physicises')
        physics_type = next(iter(physicises))
        lab = '{} tf method rel error - mc rel error'.format(physics_type)
        cb.set_label(lab, y=0.98, va='top')
        fig.savefig(name, bbox_inches='tight')


class TransferRelUncertaintyPlot(object): 
    """
    Expects to be initialized with transfer factor dictionary. 
    Hierarchy: 
        - Control Region
        - Signal Region
        - TF / error tuple
    """
    def __init__(self, transfer_factors, control_region): 
        self.transfer_factors = transfer_factors[control_region]
        self._setup_positions()
        self.dotsize = 120
    def _setup_positions(self): 
        signal_regions = set(self.transfer_factors.keys())
        self.signal_regions = sorted(signal_regions, key=cr_sort)
        ex_region = self.signal_regions[0]
        self.systematics = sorted(
            self.transfer_factors[ex_region].transfer_rel_errors)
        
        self.sr_pos = {k:p for p,k in enumerate(self.signal_regions)}
        self.syst_pos = {k:p for p,k in enumerate(self.systematics)}

    def save(self, name): 
        x = []                  # control regions
        y = []                  # signal regions
        z = []                  # something? 
        physicises = set()
        for srname, tf in self.transfer_factors.iteritems(): 
            for systematic, syst_val in tf.transfer_rel_errors.iteritems():
                y.append(self.syst_pos[systematic])
                x.append(self.sr_pos[srname])
                z.append(abs(syst_val))
            physicises.add(tf.physics)

        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)
        sc = ax.scatter(x, y, c=z, s=self.dotsize, norm=LogNorm(), 
                        vmin=1e-3, vmax=1e-1)
        ax.set_title(basename(splitext(name)[0]))
        ax.set_xticks([self.sr_pos[x] for x in self.signal_regions])
        ax.set_xticklabels([texify_sr(x) for x in self.signal_regions])
        ax.set_yticks([self.syst_pos[x] for x in self.systematics])
        ax.set_yticklabels([texify_sr(x) for x in self.systematics])
        log_locate = LogLocator(10, np.arange(0.1,1,0.1))
        log_format = LogFormatterMathtext(10)
        cb = plt.colorbar(sc, format=log_format, ticks=log_locate)
        if not len(physicises) == 1: 
            raise IOError('too many physicises')
        physics_type = next(iter(physicises))
        lab = '{} TF relative error'.format(physics_type)
        cb.set_label(lab, y=0.98, va='top')
        fig.savefig(name, bbox_inches='tight')

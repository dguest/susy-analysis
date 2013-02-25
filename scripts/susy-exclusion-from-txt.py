#!/usr/bin/env python2.7
"""
reads in an optimum cuts textfile, produces signal significance plane
"""

import sys, argparse
import matplotlib.pyplot as plt
from matplotlib.ticker import LogFormatter, LogLocator
from matplotlib.colors import LogNorm, Normalize
import matplotlib as mpl
import numpy as np

class SigPoint(object): 
    def __init__(self, mstop, mlsp, sig, region, excluded): 
        self.mstop = int(mstop)
        self.mlsp = int(mlsp)
        self.sig = float(sig)
        self.region = region
        try: 
            self.excluded = {
                'yes':True, 'no':False, 
                }[excluded]
        except KeyError: 
            self.excluded = None
    @property
    def key(self): 
        return self.mstop, self.mlsp
    

def make_sr_dict(textfile): 
    sr_dict = {}
    for line in open(textfile): 
        if line.strip().startswith('#'): 
            continue
        if not line.strip(): 
            continue
        sig_point = SigPoint(*line.strip().split())
        
        sr_dict[sig_point.key] = sig_point
    return sr_dict

class PlanePlotter(object): 
    def __init__(self, sr_dict): 
        self.x_label = r'$m_{\tilde{t}}$ [GeV]'
        self.y_label = r'$m_{\tilde{t}} - m_{\chi}$ [GeV]'
        self.cb_label = r'$\frac{s}{\sqrt{b + (\sigma b)^2} }$'
        self.vrange = None
        self.dotsize = 120
        self.x = []
        self.y = []
        self.pts = []
        self.signal_region_colors = {}
        color = iter(list('rgkwbyrgkwby') + ['pink'])
        for (mstop, mlsp), sigpoint in sr_dict.iteritems(): 
            self.x.append(mstop)
            self.y.append(mstop - mlsp)
            self.pts.append(sigpoint)
            if not sigpoint.region in self.signal_region_colors: 
                self.signal_region_colors[sigpoint.region] = next(color)
        self.sr_color_list = list('rgbwkym') + ['pink']
        

    def plot_splane(self, out_name): 
        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)

        x = self.x
        y = self.y
        z = [s.sig for s in self.pts]
    
        opt_args = {}
        if self.vrange: 
            opt_args['vmin'] = vrange[0]
            opt_args['vmax'] = vrange[1]
    
        sc = ax.scatter(x, y, 
                        c=z, 
                        norm=LogNorm(), 
                        s=self.dotsize, 
                        **opt_args)
            
        log_locate = LogLocator(10, np.arange(0.1,1,0.1))
        log_format = LogFormatter(10)
        cb = plt.colorbar(sc, format=log_format, ticks=log_locate)
        
        ax.set_xlabel(self.x_label, x=0.98, ha='right')
        ax.set_ylabel(self.y_label, y=0.98, va='top')
        cb.set_label(self.cb_label, y=0.98, va='top')
    
        fig.savefig(out_name,bbox_inches='tight')
        plt.close(fig)

    def plot_srs(self, out_name): 
        fig = plt.figure(figsize=(6,4.5))
        ax = fig.add_subplot(1,1,1)

        sr_x = {}
        sr_y = {}
        for x, y, pt in zip(self.x, self.y, self.pts): 
            if not pt.region in sr_x: 
                sr_x[pt.region] = [x]
                sr_y[pt.region] = [y]
            else: 
                sr_x[pt.region].append(x)
                sr_y[pt.region].append(y)
            
        for sr, x in sr_x.iteritems(): 
            y = sr_y[sr]
            marker = {
                'A':'^', 
                'B':'s', 
                'C':'o', 
                }[sr[0]]
            color = self.sr_color_list[int(sr[1]) - 1]
            sc = ax.scatter(x, y, 
                            c=color,  
                            s=self.dotsize, 
                            marker=marker, 
                            label=sr)
        
        ax.set_xlabel(self.x_label, x=0.98, ha='right')
        ax.set_ylabel(self.y_label, y=0.98, va='top')
        ax.set_xlim((75,500))
        legends = zip(*ax.get_legend_handles_labels())
        def get_sec(tup): 
            return tup[1]
        legends = sorted(legends, key=get_sec)
        handles, labels = zip(*legends)
        leg = ax.legend(handles, labels, scatterpoints=1, frameon=False, 
                        ncol=1)
        fig.savefig(out_name,bbox_inches='tight')
        plt.close(fig)


def run(): 
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('input_textfiles', nargs='+')
    d = 'default: %(default)s'
    parser.add_argument('--regions-name', default='sr.pdf', help=d)
    parser.add_argument('--significance-name', default='sig.pdf', help=d)
    args = parser.parse_args(sys.argv[1:])
    best_sr = {}
    for f in args.input_textfiles: 
        sr_dict = make_sr_dict(f)
        for key in sr_dict: 
            if key not in best_sr or sr_dict[key].sig > best_sr[key].sig: 
                best_sr[key] = sr_dict[key]
    
    plotter = PlanePlotter(best_sr)
    plotter.plot_splane(args.regions_name)
    plotter.plot_srs(args.significance_name)
        

if __name__ == '__main__': 
    run()

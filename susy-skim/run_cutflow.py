#!/usr/bin/env python2.7

from susy import cutflow 
import sys


normed_cutflow = cutflow.NormedCutflow('SampleListStop.txt')

sig_counts = {}
for sig in ['Stop-180-100_1excl','Stop-180-100_2incl']: 
    sig_counts[sig] = normed_cutflow.get_normed_counts(sig)
    
cuts_total = {}
for cf in sig_counts.values(): 
    if not cuts_total: 
        cuts_total = cf
    else: 
        for cut in cuts_total: 
            cuts_total[cut] += cf[cut]

sorted_cuts = sorted(cuts_total.items(), key=lambda cut: cut[1], reverse=True)
           
for name, count in sorted_cuts: 
    print '{:>20}: {: >12.2f}'.format(name,count)



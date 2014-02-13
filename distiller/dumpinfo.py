#!/usr/bin/env python2.7

from ROOT import TFile, TTree
import sys

def run(): 
    if len(sys.argv) == 1: 
        sys.exit('need files')
    for fname in sys.argv[1:]: 
        dump(fname)
    
def dump(file_name): 
    from ROOT import TFile, TTree
    ro_file = TFile(file_name)
    ct_evt = ro_file.Get('collection_tree_events').GetVal()
    print "ct events {}".format(ct_evt)
    total = ro_file.Get('total_events').GetVal()
    print "total {}".format(total)
    skimmed = ro_file.Get('skimmed_events').GetVal()
    print "skimmed {}".format(skimmed)


if __name__ == '__main__': 
    run()

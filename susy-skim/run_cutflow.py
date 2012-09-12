#!/usr/bin/env python2.7

from susy import cutflow 
import sys


# n = cutflow.NormedCutflow('SampleListStop.txt')

cutflow.print_raw_cutflow(sys.argv[1])



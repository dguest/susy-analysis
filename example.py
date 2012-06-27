#!/usr/bin/env python2.7


from jetnet import cxxprofile

ints = [('nVTX',0,10)]

cxxprofile.profile_fast(in_file='v16_jfcWithSv1.root', tree='SVTree', 
                        out_file='exampleOut.root', ints=ints, 
                        show_progress = True)


#!/usr/bin/env python2.7

from ConfigParser import SafeConfigParser
from argparse import ArgumentParser
import sys

GeV = 1e3

def run(args): 

    pt_cuts = [220, 280, 350, 500]
    ratios = [1.0, 1.3]

    if not args.sr4: 
        pt_cuts = pt_cuts[:3]

    def_out = args.c is None and not args.l

    config = SafeConfigParser()
    config.optionxform = str
    config.read([args.base])
    
    head_row = ['region']
    pt_row = [r'$\pt$ cut']
    met_row = [r'$\met$ cut']
    

    for ratio, region_name in zip(ratios,'AB'): 
        if def_out: 
            print '{} region, ratio: {}'.format( region_name, ratio)
        for subreg_n, pt_cut in enumerate(pt_cuts, 1): 
            round_met = round(pt_cut / ratio, -1)
            if def_out: 
                print 'pt: {pt}, met: {met:.3g}'.format(pt=pt_cut,met=round_met)
            head_row.append('{}{}'.format(region_name, subreg_n))
            pt_row.append('{}'.format(pt_cut))
            met_row.append('{:.3g}'.format(round_met))
    
            if 'hard_cuts' not in config.sections(): 
                config.add_section('hard_cuts')
    
            config.set('hard_cuts', 'met', str(round_met*GeV))
            config.set('hard_cuts', 'leadingPt', str(pt_cut*GeV))
            if args.anti_u: 
                for argn in xrange(2): 
                    if args.anti_u[argn] == 'ni': 
                        args.anti_u[argn] = '-inf'
                config.set('hard_cuts', 'j2Cu', args.anti_u[0])
                config.set('hard_cuts', 'j3Cu', args.anti_u[1])
                

            if args.c is not None: 
                if args.c: 
                    out_name = '{}{}-{}.cfg'.format(region_name, subreg_n, 
                                                    args.c)
                else: 
                    out_name = '{}{}.cfg'.format(region_name, subreg_n)
                with open(out_name, 'w') as out_file:
                    config.write(out_file)
    
    if args.l: 
        print r'\begin{{tabular}}{{ {} }}'.format('c'*len(head_row))
        print r'{}\\ \hline'.format(' & '.join(head_row))
        print r'{}\\ '.format(' & '.join(pt_row))
        print r'{}'.format(' & '.join(met_row))
    
if __name__ == '__main__': 
    parser = ArgumentParser()
    parser.add_argument('base', help='base config file', nargs='?', 
                        default='optimize.cfg')
    parser.add_argument('-l', action='store_true', help='latex output')
    parser.add_argument('-c', nargs='?', const='', default=None, 
                        help='make cfg files with extension C')
    parser.add_argument('--sr4', action='store_true', help='use sr 4')
    parser.add_argument('--anti-u', nargs=2, help='ni for negitive inf')
    args = parser.parse_args(sys.argv[1:])
    run(args)

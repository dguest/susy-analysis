#!/usr/bin/env python2.7
"""
uses run / mc meta data and ntuples files to produce stacks
"""

import argparse
from stop.stack.coordinator import Coordinator
from os.path import isfile
from stop.stack import table
from tempfile import TemporaryFile
import yaml
import sys


def get_config(): 
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('steering_file', help="created if it doesn't exist")
    parser.add_argument('-f','--force-aggregation', action='store_true')
    parser.add_argument('-r','--rerun-stack', action='store_true')
    parser.add_argument('-t', '--dump-tex', action='store_true')
    parser.add_argument(
        '-s','--signal-point', nargs='?', 
        const='stop-150-90', help="assumes <particle>-<something> type name")

    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"
    parser.add_argument('-p', 
        '--plots', help='directory to store plots, ' + c, nargs='?', 
        default=None, const='plots')
    parser.add_argument(
        '--ext', help='plot extensions, ' + d, default='.pdf')
    args = parser.parse_args(sys.argv[1:])
    return args

def run(): 
    args = get_config()

    if not isfile(args.steering_file): 
        coord = Coordinator()
        with open(args.steering_file,'w') as yml: 
            coord.write(yml)
            sys.exit('wrote example steering file, quitting...')

    if args.signal_point: 
        signal_head = args.signal_point.split('-')[0]
        def needed(tup): 
            phys = tup[0]
            if not phys.startswith(signal_head): 
                return True
            return phys == args.signal_point
    else: 
        def needed(tup): 
            return True

    with open(args.steering_file) as yml: 
        coord = Coordinator(yml)
    coord.stack(systematic='all', 
                rerun=args.rerun_stack)

    all_dict = coord.aggregate(systematic='all',
                               rerun=args.force_aggregation)
        
    out_file = TemporaryFile()
    count_dict = {}
    for name, syst_dict in all_dict.iteritems(): 
        syst_count = table.get_physics_cut_dict(syst_dict)
        slim_counts = {k:v for k,v in syst_count.items() if needed(k)}
        count_dict[name] = slim_counts
        table.make_latex_bg_table(slim_counts, title=name, 
                                  out_file=out_file)
    if args.dump_tex: 
        out_file.seek(0)
        for line in out_file: 
            print line.strip()
    elif not args.plots: 
        print yaml.dump(table.yamlize(count_dict))


    if args.plots: 
        # sys.exit('plotting not implemented, quitting...')
        from stop.stack import plot
        plots_dict = all_dict['NONE']
        plotting_info = coord.get_plotting_meta()
        plotting_info['output_ext'] = args.ext
        plotting_info['base_dir'] = args.plots
        plot.make_plots(plots_dict, plotting_info)
        

if __name__ == '__main__': 
    run()

#!/usr/bin/env python3.3
"""
Plotting routine for stack plots
"""
import argparse
import yaml
import sys

def get_config(): 
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"
    def_pts = ['scharm-400-200', 'scharm-550-50']
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('aggregate')
    parser.add_argument('steering_file', nargs='?')
    parser.add_argument('--scale', choices={'log','linear', 'both'}, 
                        default='log', help=d)
    parser.add_argument(
        '--filt', help='not really sure what this does... should fix it')
    parser.add_argument(
        '-s','--signal-points', default=def_pts, nargs='+',
        help="assumes <particle>-<something> type name, " + d)
    parser.add_argument(
        '--ext', help='plot extensions, ' + d, default='.pdf')
    parser.add_argument('-o', '--output-dir', 
                        help=d, default='plots')

    args = parser.parse_args(sys.argv[1:])
    return args

def run(): 
    args = get_config()
    run_plotmill(args)

def run_plotmill(args): 
    from scharm.aggregate import plot
    from scharm.aggregate.aggregator import HistDict
    config = _get_config_info(args.steering_file)
    aggregates = [args.aggregate]
    used_physics = config['backgrounds']['used'] + ['data']
    if args.signal_points: 
        used_physics.extend(args.signal_points)
    plots_dict = {}
    for agg_file in aggregates: 
        print('loading {}'.format(agg_file))
        hists = HistDict(agg_file,args.filt, physics_set=used_physics,
                         var_blacklist={'truth'})
        plots_dict.update(hists)
            
    needed = _get_signal_finder(args.signal_points)
    plots_dict = {k:v for k,v in plots_dict.items() if needed(k)}
    plotting_info = {
        'lumi_fb': config['misc']['lumi_fb'],
        'base_dir': args.output_dir, 
        'output_ext': args.ext, 
        'used_backgrounds': config['backgrounds']['used'], 
        }

    do_log = args.scale == 'log'
    plot.make_plots(plots_dict, plotting_info, log=do_log)
    if args.scale == 'both': 
        plot.make_plots(plots_dict, plotting_info, log=True)

def _get_signal_finder(signal_points): 
    if signal_points: 
        signal_head = signal_points[0].split('-')[0]
        def needed(tup): 
            phys = tup[0]
            if not phys.startswith(signal_head): 
                return True
            for point in signal_points:
                if phys == point: 
                    return True
            return False
    else: 
        def needed(tup): 
            return True
    return needed

def _get_config_info(steering_file): 
    if not steering_file: 
        # TODO: 
        #  1) get lumi from aggregate file
        #  2) make a better filter for background types
        bgs = ['diboson', 'QCD','singleTop','ttbar', 'ttbarV']
        for ch in 'BCL': 
            bgs.append('Wjets' + ch)
            bgs.append('Zjets' + ch)
        config = {
            'misc': { 'lumi_fb': 20.3 }, 
            'backgrounds': {'used': bgs }, 
            }
        return config
    else: 
        with open(steering_file) as steering_yml: 
            config = yaml.load(steering_yml)
        return config
        
if __name__ == '__main__': 
    run()

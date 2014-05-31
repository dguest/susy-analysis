#!/usr/bin/env python3.3
"""
dump regions for diagnostics / maybe to plot or display them
"""
import argparse, sys
import yaml
from scharm.stack.regions import Region, sbottom_regions

def run():
    config = get_config()
    dump_regions(config)

def get_config():
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('steering_file')
    action = parser.add_mutually_exclusive_group()
    action.add_argument('-c','--counts', action='store_true')
    return parser.parse_args(sys.argv[1:])

def dump_regions(config):
    with open(config.steering_file) as steering_yml:
        config_dict = yaml.load(steering_yml)
    regions = {k:Region(v) for k, v in config_dict['regions'].items()}
    if config.counts:
        print(len(regions))
    else:
        _print_regions(regions)

def _print_regions(regions):
    for name, reg in regions.items():
        print(name, end=':\n')
        for key, val in reg.get_config_dict().items():
            print('  {}: {}'.format(key, val))

if __name__ == '__main__':
    run()

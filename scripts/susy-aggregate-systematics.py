#!/usr/bin/env python3
"""
Script to add systematic variations to the final stack plots.
"""
_files_help = "can specify either a root directory or a set of files"

from scharm.aggregate.systematics import make_syst_aggregate
from scharm.schema import nominal_syst as nom_syst
import argparse
import sys
from os.path import isdir, isfile, basename, splitext, join
from os import listdir, remove

def get_config():
    d = 'default: %(default)s'
    c = "with no argument is '%(const)s'"

    parser = argparse.ArgumentParser(
        description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('files', nargs='+', help=_files_help)
    parser.add_argument('-o','--output', default='aggregate.h5', help=d)
    return parser.parse_args(sys.argv[1:])

def run():
    args = get_config()
    sys_files = _get_systematic_files(args.files)
    if isfile(args.output):
        remove(args.output)
    if not nom_syst in sys_files:
        raise OSError("nominal systematic '{}' not found".format(nom_syst))
    make_syst_aggregate(sys_files, args.output)

def _get_systematic_files(files):
    sys_dict = {}
    for sysfile in files:
        if isdir(sysfile):
            subfiles = [join(sysfile, sub) for sub in listdir(sysfile)]
            new_files = _get_systematic_files(subfiles)
            if any(new in sys_dict for new in new_files):
                raise OSError("double counting problem")
            sys_dict.update(new_files)
        else:
            key = splitext(basename(sysfile))[0]
            if key in sys_dict:
                raise OSError("double counted {}".format(key))
            sys_dict[key] = sysfile
    return sys_dict

if __name__ == '__main__':
    run()

import yaml
from os.path import basename, splitext, isfile
import h5py
import sys
import re
import numpy as np
from collections import Counter

class Normalizer: 
    """
    provide an iterator which returns h5 files with a normalizer function.
    """
    signal_name_template_met_filter = ( 
        '{}-{stop_mass_gev}-{lsp_mass_gev}-TMF{met_filter_gev:.0f}')
    signal_name_template = '{}-{stop_mass_gev}-{lsp_mass_gev}'

    def __init__(self,meta_path, hfiles, lumi_fb=20.3): 
        self.hfiles = hfiles
        with open(meta_path) as yml: 
            self.filter_meta = yaml.load(yml)
        self._lumi_fb = lumi_fb
        self.signal_prestring = 'scharm'
        self.outstream = sys.stdout
        self.bugstream = sys.stderr
        self.out_prepend = ''

    def _get_matched_signame(self,ds): 
        """
        this is slightly hackish, but does the right thing by renaming the 
        physics type. 
        """
        finder = re.compile('Scharm_dir_([0-9]+)_([0-9]+)_MET([0-9]+)')
        
        try: 
            found = finder.search(ds).group
        except AttributeError: 
            return None
        generator_info = { 
                'stop_mass_gev': int(found(1)), 
                'lsp_mass_gev': int(found(2)), 
                'met_filter_gev': found(3),
                }
        
        namestring = self.signal_name_template
        return namestring.format(self.signal_prestring,
                                 **generator_info)

    def _get_physics_type(self, file_meta): 
        full_name = file_meta['full_name']
        if not 'physics_type' in file_meta:
            if full_name.startswith('data'):
                physics_type = 'data'
            else: 
                raise OSError('got unknown physics in {}'.format(full_name))
        else:
            physics_type = file_meta['physics_type']

        if 'signal' in physics_type:
            physics_type = self._get_matched_signame(full_name)
        if not physics_type: 
            raise OSError("couldn't classify {}".format(full_name))
        return physics_type

    def _check_for_bugs(self, ds): 
        full_name = ds['full_name']
        if not 'total_xsec_fb' in ds and not full_name.startswith('data'):
            self.bugstream.write(
                'no cross section for {}, skipping\n'.format(full_name))
            return True

        if 'n_corrupted_files' in ds:
            self.bugstream.write(
                '{} bad files in {}\n'.format(ds['n_corrupted_files'],
                                              full_name))
            
        return False

    def _get_hist_scaler(self, file_meta): 
        """
        Factory of scalar factories: the returned function 
        returns scalar after it's been called on the h5 file
        """
        ds_name = file_meta['full_name']

        if ds_name.startswith('data'): 
            def scalar_fact(hfile): 
                return 1.0
        else: 
            filteff = file_meta['filteff']
            xsec = file_meta['total_xsec_fb']
            kfactor = file_meta.get('kfactor',1)
            n_before_sel = xsec * kfactor * filteff * self._lumi_fb
            def scalar_fact(hfile): 
                sum_evt_weight = hfile.attrs['total_event_weight']
                return n_before_sel / sum_evt_weight
        return scalar_fact

    def _print_prog(self, filenum, numfiles): 
        if self.outstream and self.outstream.isatty():
            self.outstream.write(
                '\r{}adding file {} of {}'.format(
                    self.out_prepend, filenum, numfiles))
            self.outstream.flush()

    def __iter__(self): 
        """
        iterates over files, gets (physics_type, file, normalization) tuples
        """
        stats = StatsCounter()

        numfiles = len(self.hfiles)
        for filenum, f in enumerate(self.hfiles):
            self._print_prog(filenum, numfiles)
            meta_name = basename(splitext(f)[0])
    
            file_meta = self.filter_meta[meta_name]
            if self._check_for_bugs(file_meta): 
                continue

            scaler_fact = self._get_hist_scaler(file_meta)
            physics_type = self._get_physics_type(file_meta)
    
            with h5py.File(f,'r') as hfile: 
                stats.update(physics_type, file_meta, hfile)
                norm = scaler_fact(hfile)
                yield physics_type, hfile, norm
                
        if self.outstream and self.outstream.isatty(): 
            self.outstream.write('\n')
        stats.write_to(self.bugstream)

class StatsCounter: 
    """
    Counters to keep track of the number of expected events vs the number of 
    events actually found
    """
    out_str = '{pt}: {ex} of {fo} ({ra:.2%})\n'
    def __init__(self): 
        self._expected_entries = Counter()
        self._found_entries = Counter()
    def update(self, phys_type, meta, hfile): 
        expected = meta.get('n_expected_entries', 0)
        found = hfile.attrs['total_events']
        self._expected_entries[phys_type] += expected
        self._found_entries[phys_type] += found
    def write_to(self, out_file): 
        for phys_type, found in self._found_entries.items(): 
            expected = self._expected_entries[phys_type]
            out_vals = dict(
                ex=expected,
                ra=(found / expected),
                pt=phys_type, fo=found)
                            
            out_file.write(self.out_str.format(**out_vals))

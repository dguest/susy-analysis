import re
from scharm.meta import DatasetCache
from warnings import warn

class MetaFactory(object): 
    """
    This pulls together: 
     - steering list
     - susy xsection / n events info

    and generates the meta file, which is used to steer everything up to 
    histogram generation. 

    Should work on stripping this down and making it into a susy 
    lookup class. 
    """
    def __init__(self, steering=None): 
        """
        Initalize from either: 
         - a text file list of datasets
         - an existing meta file
        """
        self._datasets = None
        if steering is None: 
            self._datasets = DatasetCache()
        elif steering.endswith('.pkl') or steering.endswith('.yml'): 
            self._datasets = self._build_from_meta(steering)
        elif steering.endswith('.txt'): 
            with open(steering) as ds_list:
                self.add_ugly_ds_list(ds_list)
        else: 
            raise ValueError('{} is an unsupported file for MetaFactory'
                             'constructor')
        self.clear_ami = False
        self.no_ami_overwrite = False
        self.verbose = False

    def _build_from_meta(self, meta_file_name): 
        return DatasetCache(meta_file_name)

    @property
    def datasets(self): 
        return self._datasets

    def lookup_susy(self, susy_file): 
        """
        Matches whatever datasets have a cross section listed in the 
        susy file.
        """
        id_line = ''
        split_dict = {}
        for line in susy_file: 

            # first line of the susy textfile is some kind of 
            # field description         
            if not id_line: 
                id_line = line
                continue

            clean_line = line.split('#')[0].strip()
            if not clean_line: 
                continue

            spl = clean_line.split()
            try: 
                name = spl[1]
                ds_id = int(spl[0])
            except IndexError: 
                if 'http' in clean_line: 
                    warn('some fucktard stuck http in the file without'
                         ' commenting it out... skipping line', 
                         stacklevel=2)
                    continue
                raise ValueError("can't parse {}".format(clean_line))
            split_dict[ds_id] = spl
            

        for ds in self._datasets.values(): 
            if ds.id in split_dict: 
                spl = split_dict[ds.id]
                ds.id = int(spl[0])
                ds.name = spl[1]
                ds.total_xsec_fb = float(spl[2]) * 1e3
                ds.kfactor = float(spl[3])
                ds.filteff = float(spl[4])
                ds.meta_sources.add('susy lookup')
    
    def _print(self, print_string): 
        if self.verbose: 
            print print_string

    def write_meta(self, output_file_name): 
        """
        write out the meta info
        """
        self._datasets.write(output_file_name)


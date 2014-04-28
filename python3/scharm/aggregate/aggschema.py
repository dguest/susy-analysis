import h5py
from os.path import isfile
from scharm.hists import HistNd

class HistDict(dict):
    """
    Dictionary with some methods for HDF5 persistence.

    Keyed by (physics, variable, region) tuples.

    The persistent format is actually rather ugly:
        - top dir is physics type
        - bottom dir is cut / region
        - all middle dirs are the variable name
    """
    def __init__(self, file_name='', filt=None, sig_prefix='scharm',
                 sig_points=[],
                 cut_set=None, var_blacklist=None):
        if not file_name:
            return None
        with h5py.File(file_name,'r') as infile:
            for proc, var_grp in infile.items():
                if proc.startswith(sig_prefix):
                    if proc not in sig_points:
                        continue
                for subpath in self._list_paths(var_grp):
                    path = proc + subpath
                    if filt:
                        if not filt in path:
                            continue
                    spl = path.split('/')
                    physics = spl[0]
                    if var_blacklist and set(spl[1:-1]) & var_blacklist:
                        continue
                    variable = '/'.join(spl[1:-1])
                    cut = spl[-1]
                    if cut_set and cut not in cut_set:
                        continue
                    nametup = (physics, variable, cut)
                    hist = infile[path]
                    self[nametup] = HistNd(hist)

    def __setitem__(self, key, value):
        if len(key) != 3 or not all(isinstance(k,str) for k in key):
            raise TypeError("key {} isn't right for HistDict".format(key))
        if any(len(s) == 0 for s in key):
            raise ValueError("zero-length strings prohibited: {}".format(key))
        super(HistDict,self).__setitem__(key, value)

    def _list_paths(self, group, prefix=''):
        paths = []
        for name, val in group.items():
            newprefix = '/'.join([prefix, name])
            if isinstance(val, h5py.Group):
                paths += self._list_paths(val, newprefix)
            else:
                paths.append(newprefix)
        return paths

    def write(self, file_name):
        if isfile(file_name):
            raise IOError('refusing to overwrite {}'.format(file_name))
        with h5py.File(file_name,'w') as outfile:
            for nametup, ndhist in self.items():
                physics, variable, cut = nametup
                path = '/'.join([''] + list(nametup))
                ndhist.write_to(outfile, path)


import glob
import h5py
from stop.hists import HistNd
from os.path import basename, splitext, isdir
import os
from stop.plot.efficiency import EfficiencyPlot

class JetEfficiencyPlotter(object): 
    _group_name = 'alljet'
    def __init__(self): 
        self.group_name = self._group_name 

    def _process_hist(self, hist_nd): 
        """
        converts the n-dim hist into an (array,extent) tuple, 
        which is the expected input for EfficiencyPlot
        """
        units = hist_nd.axes['x'].units
        raw_array, extent = hist_nd.project_1d('x')
        factor = {'MeV':1e-3, 'GeV':1.0}[units]
        extent = [e*factor for e in extent]
        return raw_array[1:-1], extent

    def _get_arrays(self, hist_group, tags): 
        """
        returns a dict of tuples of the form {name: (num, denom, extent)}.
        """
        all_hist = HistNd(hist_group['all'])
        tags = {tag: HistNd(hist_group[tag]) for tag in tags}
        all_array, all_ext = self._process_hist(all_hist)
        numerator_arrays = {}
        for tag, num in tags.iteritems(): 
            num_array, num_ext = self._process_hist(num)
            if not num_ext == all_ext: 
                raise ValueError(
                    'hist extents unequal: {} != {}'.format(num_ext, all_ext))
            numerator_arrays[tag] = num_array
        out_dict = {}
        for tag, num_array in numerator_arrays.iteritems(): 
            out_dict[tag] = (num_array, all_array, all_ext)
        return out_dict

    def _get_tuple_dict(self, sample_names, tags='all', flavors='all'): 
        plot_tuples = {}
        for sample in sample_names: 
            shortsample = basename(splitext(sample)[0])
            with h5py.File(sample) as h5_file: 
                try: 
                    flavor_group = h5_file[self._group_name]
                except KeyError as err: 
                    keys = ', '.join(h5_file.keys())
                    err = "error opening '{}' in '{}' possible keys: {}"
                    raise IOError(err.format(self._group_name, sample, keys))
                if flavors == 'all': 
                    flavors = flavor_group.keys()
                for flavor in flavors: 
                    tag_group = flavor_group[flavor]
                    if tags == 'all': 
                        tags = tag_group.keys()
                    tag_dict = self._get_arrays(tag_group, tags)
                    for tag, tup in tag_dict.iteritems(): 
                        plot_tuples[shortsample, flavor, tag] = tup
        return plot_tuples
    
    def _get_sft(self, tup_dict): 
        return tuple(set(x) for x in zip(*tup_dict.keys()))
    
    def plot_samples(self, sample_names, tags='all', flavors='all', 
                     out_dir='plots'): 
        if not isdir(out_dir): 
            os.mkdir(out_dir)
        tuple_dict = self._get_tuple_dict(sample_names, tags, flavors)
        all_samples, all_flavors, all_tags = self._get_sft(tuple_dict)
        if tags == 'all': 
            tags = all_tags
        if flavors == 'all': 
            flavors = all_flavors
        for flavor in flavors: 
            for tag in tags: 
                def selector(key_tup): 
                    s, f, t = key_tup
                    return f == flavor and t == tag
                pl_keys = {k for k in tuple_dict if selector(k)}
                eff_plot = EfficiencyPlot(0.0, 1.0)
                eff_plot.ax.set_ylabel('{} tag efficiency'.format(flavor), 
                                       y=0.98, va='top')
                eff_plot.ax.set_xlabel(r'Jet $p_{\mathrm{T}}$ [GeV]', 
                                       x=0.98, ha='right')
                for key in pl_keys: 
                    name = key[0]
                    eff_plot.add_efficiency(*tuple_dict[key], 
                                            name=name)
                plot_name = '{}/{}-{}.pdf'.format(out_dir, flavor, tag)
                eff_plot.save(plot_name)

import glob
import h5py
from stop.hists import HistNd
from os.path import basename, splitext, isdir
import os
from stop.plot.efficiency import EfficiencyPlot

class JetEfficiencyPlotter(object): 
    _group_name = 'alljet'
    _wt2_append = 'Wt2'
    def __init__(self, do_wt2_error=True): 
        self.group_name = self._group_name 
        self.max_pt_gev = 400.0
        self.custom_ranges = { 
            ('charm','loose'): (0.9, 1.1), 
            ('charm','medium'): (0.0, 0.3), 
            ('bottom','medium'): (0.0, 0.4), 
            }
        self.custom_colors = { 
            'Powheg': 'r', 
            'Sherpa-LeptHad.h5': 'b', 
            'Sherpa-TauleptHad.h5': 'k', 
            'McAtNlo': 'g'
            }
            
        self.do_wt2_error = do_wt2_error
        self.x_range_gev = (0.0, 400.0)

    def _get_color(self, name): 
        for match in self.custom_colors: 
            if match in name: 
                return self.custom_colors[match]
        return None

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
        """
        the returned hists will have units of GeV
        """
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
            tags = [tag for tag in all_tags if not tag.endswith('Wt2')]
        if flavors == 'all': 
            flavors = all_flavors
        for flavor in flavors: 
            for tag in tags: 
                def selector(key_tup): 
                    s, f, t = key_tup
                    return f == flavor and t == tag
                pl_keys = {k for k in tuple_dict if selector(k)}
                if (flavor, tag) in self.custom_ranges: 
                    y_range = self.custom_ranges[flavor, tag]
                else: 
                    y_range = (0.0, 1.0)
                eff_plot = EfficiencyPlot(x_range=self.x_range_gev, 
                                          y_range=y_range)
                eff_plot.ax.set_ylabel('{} tag efficiency'.format(flavor), 
                                       y=0.98, va='top')
                eff_plot.ax.set_xlabel(r'Jet $p_{\mathrm{T}}$ [GeV]', 
                                       x=0.98, ha='right')
                for key in pl_keys: 
                    name = key[0]
                    wt2_tag = key[2] + self._wt2_append
                    num_wt2 = denom_wt2 = None
                    if self.do_wt2_error: 
                        num_wt2, denom_wt2, wt_ext = tuple_dict[
                            name, flavor, wt2_tag]
                    color = self._get_color(name)
                    eff_plot.add_efficiency(*tuple_dict[key], 
                                             name=name, color=color, 
                                             num_wt2=num_wt2, 
                                             denom_wt2=denom_wt2)

                plot_name = '{}/{}-{}.pdf'.format(out_dir, flavor, tag)
                eff_plot.save(plot_name)

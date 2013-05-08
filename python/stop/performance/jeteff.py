import glob
import h5py
from stop.hists import HistNd
from os.path import basename, splitext, isdir
import os
from stop.plot.efficiency import EfficiencyPlot, BinnedEfficiencyPlot

class JetEfficiencyPlotter(object): 
    _group_name = 'alljet'
    _wt2_append = 'Wt2'
    _colors = ['black','red','blue','green','purple', 'cyan']
    _jetfitter_pt_bins_gev = [25.0, 35.0, 50.0, 80.0, 120.0, 200.0]
    _scalefactor_pt_bins_gev = [
        25.0, 30.0, 40.0, 50.0, 60.0, 75.0, 90.0, 110.0, 140.0, 200.0, 300.0]
    _max_uniform_bins = 150
    def __init__(self, do_wt2_error=True, draw_bins='jf'): 
        self.group_name = self._group_name 
        self.draw_bins = draw_bins
        self.max_pt_gev = 400.0
        self.custom_ranges = { 
            ('charm','loose'): (0.9, 1.1), 
            ('charm','medium'): (0.0, 0.3), 
            ('bottom','medium'): (0.0, 0.4), 
            ('light', 'medium'): (0.0, 0.03), 
            }
        self.custom_colors = { 
            'Powheg': 'r', 
            'Sherpa-LeptHad.h5': 'b', 
            'Sherpa-TauleptHad.h5': 'k', 
            'McAtNlo': 'g', 
            }
        self.color_bank = {}
        self.color_itr = iter(self._colors)
            
        self.do_wt2_error = do_wt2_error
        self.x_range_gev = (0.0, 400.0)
        self.max_bins = 150

    def _get_color(self, name): 
        if not name in self.color_bank: 
            self.color_bank[name] = next(self.color_itr)
        return self.color_bank[name]

    def _process_hist(self, hist_nd, include_overflow=False): 
        """
        converts the n-dim hist into an (array,extent) tuple, 
        which is the expected input for EfficiencyPlot
        """
        units = hist_nd.axes['x'].units
        raw_array, extent = hist_nd.project_1d('x')
        if not include_overflow: 
            array = raw_array[1:-1]
        else: 
            array = raw_array
        factor = {'MeV':1e-3, 'GeV':1.0}[units]
        extent = [e*factor for e in extent]
        return array, extent

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

    def _is_numerator(self, tag): 
        conditions = [
            not tag.endswith(self._wt2_append), 
            tag != 'all']
        return all(conditions)

    def _add_bins(self, eff_plot): 
        from matplotlib.lines import Line2D
        if not self.draw_bins: 
            return 
        if self.draw_bins == 'jf': 
            opts = dict(linestyle = ':', color = 'red')
            name = 'JetFitter bins'
        elif self.draw_bins == 'sf': 
            opts = dict(linestyle = ':', color = 'green')
            name = 'SF bins'
        for val_gev in self._jetfitter_pt_bins_gev: 
            eff_plot.ax.axvline(val_gev, **opts)
        eff_plot.legends.append( (Line2D([0],[0],**opts), name)) 

    def plot_samples(self, sample_names, tags='all', flavors='all', 
                     out_dir='plots', plotter=BinnedEfficiencyPlot): 
        if not isdir(out_dir): 
            os.mkdir(out_dir)
        tuple_dict = self._get_tuple_dict(sample_names, tags, flavors)
        all_samples, all_flavors, all_tags = self._get_sft(tuple_dict)
        if tags == 'all': 
            tags = [tag for tag in all_tags if self._is_numerator(tag)]
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

                # TODO: figure out how to make this call slightly less ugly: 
                #   lots of the current args are ignored so that we can 
                #   stick in multiple plotters here. 
                eff_plot = plotter(x_range=self.x_range_gev, 
                                   y_range=y_range, 
                                   bins=self._scalefactor_pt_bins_gev, 
                                   max_bins=self._max_uniform_bins)
                eff_plot.ax.set_ylabel('{} tag efficiency'.format(flavor), 
                                       y=0.98, va='top')
                eff_plot.ax.set_xlabel(r'Jet $p_{\mathrm{T}}$ [GeV]', 
                                       x=0.98, ha='right')
                for key in pl_keys: 
                    name = key[0]
                    wt2_tag = key[2] + self._wt2_append
                    num_wt2 = denom_wt2 = None
                    if self.do_wt2_error: 
                        wt2_key = (name, flavor, wt2_tag)
                        num_wt2, denom_wt2, wt_ext = tuple_dict[wt2_key]
                        
                    color = self._get_color(name)
                    eff_plot.add_efficiency(*tuple_dict[key], 
                                             name=name, color=color, 
                                             num_wt2=num_wt2, 
                                             denom_wt2=denom_wt2)
                self._add_bins(eff_plot)
                plot_name = '{}/{}-{}.pdf'.format(out_dir, flavor, tag)
                eff_plot.save(plot_name)


                
                
    

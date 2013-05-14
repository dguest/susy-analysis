import glob
import h5py
from stop.hists import HistNd, HistToBinsConverter
from os.path import basename, splitext, isdir
import os, sys
from stop.plot.efficiency import EfficiencyPlot, BinnedEfficiencyPlot
from stop.plot.efficiency import RatioPlot
import itertools
from xml.etree import ElementTree as etree
from xml.dom import minidom
from stop.performance.style import get_jet_eff_color

class JetEfficiencyBase(object): 
    _group_name = 'alljet'
    _jetfitter_pt_bins_gev = [25.0, 35.0, 50.0, 80.0, 120.0, 200.0]
    _scalefactor_pt_bins_gev = [
        25.0, 30.0, 40.0, 50.0, 60.0, 75.0, 90.0, 110.0, 140.0, 200.0, 300.0]
    _wt2_append = 'Wt2'

    def __init__(self, **kwargs): 
        super(JetEfficiencyBase, self).__init__(**kwargs)
        self.group_name = self._group_name

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

    def _shortsample(self, samp_path): 
        return basename(splitext(samp_path)[0])
    
    def get_tuple_dict(self, sample_names, tags='all', flavors='all'): 
        """
        Digs into files given by 'sample_names'. Expects the following 
        hierarchy: 
         - group_name (class variable)
         - flavor truth tag
         - flavor tag (i.e. medium, loose)

        The last group also includes hists with 'Wt2' suffixes. 

        The returned dict is keyed by (sample, flavor, tag), the values
        are tuples of the form (passing_count, all_count, extent), where 
        extent has units of GeV. 
        
        Notes: Overflow and underflow bins are stripped off count arrays. 

        TODO: consider adding a level to the group hierarchy to get rid of 
        the ugly 'Wt2' suffixes. Would require some work downstream too. 
        """
        plot_tuples = {}
        for sample in sample_names: 
            shortsample = self._shortsample(sample)
            with h5py.File(sample) as h5_file: 
                try: 
                    flavor_group = h5_file[self.group_name]
                except KeyError as err: 
                    keys = ', '.join(h5_file.keys())
                    err = "error opening '{}' in '{}' possible keys: {}"
                    raise IOError(err.format(self.group_name, sample, keys))
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

class JetPlotBase(object): 
    """
    Adds _get_color and _add_bins (which in turn adds vertical bins)
    TODO: get rid of this as a baseclass (could just be a component)
    """
    _colors = ['black','red','blue','green','purple', 'cyan']
    _max_uniform_bins = 150

    def __init__(self, draw_bins='jf', **kwargs): 
        super(JetPlotBase, self).__init__(**kwargs)
        self.color_bank = {}
        self.color_itr = iter(self._colors)
        self.draw_bins = draw_bins

    def _get_color(self, name): 
        color = get_jet_eff_color(name)
        if color: 
            return color
        if not name in self.color_bank: 
            self.color_bank[name] = next(self.color_itr)
        return self.color_bank[name]

    def _add_bins(self, eff_plot): 
        from matplotlib.lines import Line2D
        if not self.draw_bins: 
            return 
        if self.draw_bins == 'jf': 
            opts = dict(linestyle = ':', color = 'red')
            name = 'JetFitter bins'
            bin_values = self._jetfitter_pt_bins_gev
        elif self.draw_bins == 'sf': 
            opts = dict(linestyle = ':', color = 'green')
            name = 'SF bins'
            bin_values = self._scalefactor_pt_bins_gev

        for val_gev in bin_values: 
            eff_plot.ax.axvline(val_gev, **opts)
        eff_plot.legends.append( (Line2D([0],[0],**opts), name)) 

class JetEfficiencyPlotter(JetPlotBase, JetEfficiencyBase): 
    def __init__(self, do_wt2_error=True, draw_bins='jf'): 
        super(JetEfficiencyPlotter, self).__init__(draw_bins)
        self.max_pt_gev = 400.0
        self.custom_ranges = { 
            ('charm','loose'): (0.9, 1.1), 
            ('charm','medium'): (0.0, 0.3), 
            ('bottom','medium'): (0.0, 0.4), 
            ('light', 'medium'): (0.0, 0.03), 
            ('tau', 'antiloose'): (0.0, 0.25), 
            ('charm', 'antiloose'): (0.0, 0.10), 
            ('light', 'antiloose'): (0.0, 0.05), 
            ('tau', 'medium'): (0.0, 0.20), 
            }
        self.custom_colors = { 
            'Powheg': 'r', 
            'Sherpa-LeptHad.h5': 'b', 
            'Sherpa-TauleptHad.h5': 'k', 
            'McAtNlo': 'g', 
            }
            
        self.do_wt2_error = do_wt2_error
        self.x_range_gev = (0.0, 400.0)
        self.max_bins = 150

    def plot_samples(self, sample_names, tags='all', flavors='all', 
                     out_dir='plots', plotter=BinnedEfficiencyPlot, 
                     plot_extension='.pdf'): 
        if not isdir(out_dir): 
            os.mkdir(out_dir)
        tuple_dict = self.get_tuple_dict(sample_names, tags, flavors)
        all_samples, all_flavors, all_tags = self._get_sft(tuple_dict)
        if tags == 'all': 
            tags = [tag for tag in all_tags if self._is_numerator(tag)]
        if flavors == 'all': 
            flavors = all_flavors

        for flavor, tag in itertools.product(flavors, tags): 
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
            plot_name = '{}/{}-{}{}'.format(out_dir, flavor, tag, 
                                            plot_extension)
            eff_plot.save(plot_name)

class JetEffRatioCalc(JetEfficiencyBase): 
    """
    Produces jet efficiency ratio plots binned by pt. At the moment
    expects histograms to have a cooresponding 'Wt2' hist in the same 
    Group. 
    """
    def __init__(self): 
        super(JetEffRatioCalc, self).__init__()
        self.binner = HistToBinsConverter(self._scalefactor_pt_bins_gev)

    def _get_x_xerr_y_yerr(self, sampdic, key): 
        """
        looks up values and wt2 hists in sampdic, returns efficiency 
        with errors. 
        """
        n_pass, n_all, extent = sampdic[key]
        # need some magic to get the sum weights
        wt_key = key[0], key[1], key[2] + self._wt2_append
        pass_wt2, all_wt2, extent = sampdic[wt_key]

        # we don't need all the binner's return values
        def get_x_xerr_y(array, extent, do_x=False): 
            under, x, x_err, y, over = self.binner.get_bin_counts(
                array, extent, overflows_included=False)
            if do_x: 
                return x, x_err, y
            else: 
                return y
        x, xerr, pass_b = get_x_xerr_y(n_pass, extent, do_x=True)
        all_b = get_x_xerr_y(n_all, extent)
        pass_wt2_b = get_x_xerr_y(pass_wt2, extent)
        all_wt2_b = get_x_xerr_y(all_wt2, extent)
        
        p = pass_b
        f = all_b - pass_b
        # use nan when we have zero stats (suppresses warnings)
        f[(f == 0.0) & (p == 0.0)] = float('nan')
        p_wt2 = pass_wt2_b
        f_wt2 = all_wt2_b - pass_wt2_b
        y_err = (p_wt2**0.5 * f + f_wt2**0.5 * p) / (p + f)**2.0
        y = p / (p + f)
        return x, xerr, y, y_err

    def get_ratios(self, num_samples, denom_samples): 
        """
        Returns a dict, keyed by (num_sample, denom_sample, flav, tag), 
        holding another dict keyed by {x_center, x_error, y_center, y_error}
        """
        all_samples = set(num_samples) | set(denom_samples)
        # gets a dictionary with values of the form (num, denom, extent)
        samples_dict = self.get_tuple_dict(all_samples)
        samples, flavors, all_tags = self._get_sft(samples_dict)
        tags = [tag for tag in all_tags if self._is_numerator(tag)]
        
        short_num = {self._shortsample(s) for s in num_samples}
        short_denom = {self._shortsample(s) for s in denom_samples}

        rat_dict = {}

        for flavor, tag in itertools.product(flavors, tags): 
            ordered_pairs = set()
            for s1, s2 in itertools.product(short_num, short_denom): 
                first = (s1, flavor, tag)
                second = (s2, flavor, tag)
                ordered_pairs.add( (first, second))
            
            binner = HistToBinsConverter(self._scalefactor_pt_bins_gev)
            for numkey, denomkey in ordered_pairs: 
                numx, numxerr, numy, numyerr = self._get_x_xerr_y_yerr(
                    samples_dict,numkey)
                denx, denxerr, deny, denyerr = self._get_x_xerr_y_yerr(
                    samples_dict, denomkey)

                ratio = numy / deny
                # assume errors are uncorrelated
                rel_err = (
                    (numyerr / numy)**2 + (denyerr / deny)**2)**0.5 
                ratio_error = rel_err * ratio
                rat_dict[numkey[0], denomkey[0], flavor, tag] = { 
                    'x_center': numx, 'x_error': numxerr, 
                    'y_center': ratio, 'y_error': ratio_error }
        return rat_dict
                    
class JetRatioPlotter(object): 
    """
    Wrapper which prints a lot of RatioPlots. 
    """
    def __init__(self, ratio_dict): 
        self.ratio_dict = ratio_dict
        self.sample_replacements = {
            'McAtNloJimmy_ttbar_LeptonFilter': r'McAtNloJimmy $t\bar{t}$', 
            }
        self.y_range = (0.7, 2.0)
        self.special_tag_ranges = {
            'tau': (0.0, 2.0), 
            'light': (0.0, 2.0), 
            }
    def plot_all_ratios(self, plot_dir): 
        for (num, denom, flav, tag), values in self.ratio_dict.iteritems(): 
            out_path = '{}/{}-over-{}-{}Tag-{}Truth.pdf'.format(
                plot_dir, num, denom, tag, flav)
            sys.stderr.write('plotting {}\n'.format(out_path))
            y_range = self.special_tag_ranges.get(flav, self.y_range)
            plotter = RatioPlot(y_range=y_range)
            plotter.add_ratio(values)
            plotter.ax.set_ylabel('{} tag efficiency'.format(flav), 
                                  y=0.98, va='top')
            plotter.ax.set_xlabel(r'Jet $p_{\mathrm{T}}$ [GeV]', 
                                  x=0.98, ha='right')
            plotter.save(out_path)

    def overlay_denominators(self, plot_dir, plot_extension='.pdf'): 
        nums, denoms, flavs, tags = (
            set(s) for s in zip(*self.ratio_dict.keys()))
        for flav, tag, num in itertools.product(flavs, tags, nums): 
            plot_name = '{}/{}-over-all-{}Tag-{}Truth{}'.format(
                plot_dir, num, tag, flav, plot_extension)
            sys.stderr.write('plotting {}\n'.format(plot_name))
            fancy_num = self.sample_replacements.get(num,num)
            y_range = self.special_tag_ranges.get(flav, self.y_range)
            plotter = RatioPlot(
                y_range=y_range, 
                legend_title='{} / ... '.format(fancy_num), 
                color_function=get_jet_eff_color)
            plotter.ax.set_ylabel(
                'eff ratio, {} {} tag efficiency'.format(
                    tag, flav), y=0.98, va='top')
            plotter.ax.set_xlabel(r'Jet $p_{\mathrm{T}}$ [GeV]', 
                                  x=0.98, ha='right')
            for denom in sorted(denoms): 
                values = self.ratio_dict[num, denom, flav, tag]
                plotter.add_ratio(values, name=denom)
            plotter.save(plot_name)
            
class JetRatioDumper(object): 
    def __init__(self, ratio_dict): 
        self.ratio_dict = ratio_dict

    def _pt_binned_list(self, num, denom, flavor, tag): 
        """
        looks for value {x_center, x_error, y_center, y_error}
        """
        values = self.ratio_dict[num, denom, flavor, tag]
        x_centers = values['x_center']
        y_centers = values['y_center']
        x_errors = values['x_error']
        y_errors = values['y_error']
        pt_bins = []
        for iii in xrange(len(x_centers)): 
            pt_bin = {
                'pt_min':float(x_centers[iii] - x_errors[iii]), 
                'pt_max':float(x_centers[iii] + x_errors[iii]), 
                'correction': float(y_centers[iii]), 
                'error': float(y_errors[iii]) }
            pt_bins.append(pt_bin)
        return pt_bins

    def as_nested_dict(self): 
        """
        Quite easy to convert to yaml. 
        """
        nums, denoms, flavs, tags = (set(s) for s in zip(*self.ratio_dict))
        num_dict = {}
        for num in nums: 
            denom_dict = {}
            for denom in denoms: 
                tag_dict = {}
                for tag in tags: 
                    flavor_dict = {}
                    for flavor in flavs: 
                        flavor_dict[str(flavor)] = self._pt_binned_list(
                            num, denom, flavor, tag)
                    tag_dict[str(tag)] = flavor_dict
                denom_dict[str(denom)] = tag_dict
            num_dict[str(num)] = denom_dict
        return num_dict
            
    def as_xml(self): 
        nums, denoms, flavs, tags = (set(s) for s in zip(*self.ratio_dict))
        root = etree.Element('corrections')
        for num in nums: 
            num_el = etree.SubElement(root, 'numerator_generator')
            num_el.attrib['name'] = num
            for denom in denoms: 
                denom_el = etree.SubElement(num_el, 'denominator_generator')
                denom_el.attrib['name'] = denom
                for tag in tags: 
                    tag_el = etree.SubElement(denom_el, 'tag')
                    tag_el.attrib['name'] = tag
                    for flavor in flavs: 
                        flav_el = etree.SubElement(tag_el, 'flavor')
                        flav_el.attrib['name'] = flavor
                        pt_list = self._pt_binned_list(
                            num, denom, flavor, tag)

                        for pt_bin in pt_list:
                            pt_node = etree.SubElement(flav_el, 'bin')
                            pt_node.attrib['min'] = str(pt_bin['pt_min'])
                            pt_node.attrib['max'] = str(pt_bin['pt_max'])
                            pt_value = etree.SubElement(pt_node, 'correction')
                            pt_value.text = str(pt_bin['correction'])
                            pt_error = etree.SubElement(pt_node, 'error')
                            pt_error.text = str(pt_bin['error'])
                
        string = etree.tostring(root)
        reparsed = minidom.parseString(string)
        return reparsed.toprettyxml(indent="  ")

    def _raw_txt_from(self, pt_bin): 
        pt_min = pt_bin['pt_min']
        pt_max = pt_bin['pt_max']
        correction = pt_bin['correction']
        error = pt_bin['error']
        string = ' '.join(['{:> 8.0f}']*2 + ['{:> 8.3f}']*2).format(
            pt_min, pt_max, correction, error)
        return string

    def _get_max_width(self, strings, buff=2): 
        return max(len(x) for x in strings) + buff

    def as_raw_text(self): 
        """
        return a list of formated lines
        """
        nums, denoms, flavs, tags = (set(s) for s in zip(*self.ratio_dict))
        lens = {
            'num': self._get_max_width(nums), 
            'den': self._get_max_width(denoms), 
            'fla': self._get_max_width(flavs), 
            'tag': self._get_max_width(tags), 
            'min': 8, 
            'cor': 8, 
            }
        lines = []
        line = ('{:{num}} {:{den}} {:{fla}} {:{tag}}' 
                ' {:>{min}} {:>{min}} {:>{cor}} {:>{cor}}'
                ).format(
            '# numerator', 'denominator', 'flavor', 'tag', 'pt_min', 
            'pt_max', 'sf', 'err', **lens)
        lines.append(line)
        for num, denom, flavor, tag in itertools.product(
            nums, denoms, flavs, tags): 
            pt_list = self._pt_binned_list(
                num, denom, flavor, tag)
            for pt_bin in pt_list: 
                line = '{:<{num}} {:<{den}} {:<{fla}} {:<{tag}} '.format(
                    num, denom, flavor, tag, **lens)
                lines.append(line + self._raw_txt_from(pt_bin))
        return lines

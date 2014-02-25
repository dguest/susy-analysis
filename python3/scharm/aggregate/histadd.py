from scharm.hists import HistNd, HistAdder
import h5py

def hadd(config): 
    """
    Scales to 1 fb^-1 if scaling is requested. 
    """
    if config.recursive: 
        _recursive_hadd(config)
        return 
    good_files = _get_good_files(config.input_hists)
    if config.dash_hadd: 
        if config.norm: 
            raise ValueError('normalization not allowed for dash-hadd')
        _dash_hadd(good_files, config.output, fast=config.fast, 
                   aggressive=config.aggressive)
    else: 
        weights_dict = {}
        if config.norm: 
            raise ValueError("normalization not currently supported...")
        # NOTE: this was commented out because we're phasing out the meta
        #       wrapper.
            # lookup = meta.DatasetCache(config.norm)
            # for in_file in good_files: 
            #     file_key = basename(splitext(in_file)[0])
            #     eff_lumi = lookup[file_key].get_effective_luminosity_fb()
            #     weights_dict[in_file] = 1.0/eff_lumi
        _hadd(good_files, config.output, weights_dict, fast=config.fast)

def _get_good_files(input_hists): 
    good_files = []
    for hist_file in input_hists: 
        if not hist_file.endswith('.h5'): 
            raise OSError("unrecognized extension: {}".format(
                    splitext(hist_file)[-1]))
        with h5py.File(hist_file, 'r') as h5: 
            if len(h5.keys()): 
                good_files.append(hist_file)
    if len(good_files) != len(input_hists): 
        sys.stderr.write(
            'ACHTUNG: only {} of {} files have any hists\n'.format(
                len(good_files), len(input_hists)))
    return good_files

def _recursive_hadd(config): 
    if not all(isdir(x) for x in config.input_hists): 
        raise OSError("recursive hadd requires input_hists to be dir")
    all_walk = chain(*(os.walk(x) for x in config.input_hists))
    for dirpath, dirnames, file_names in all_walk: 
        if not file_names: 
            continue
        out_path = join(config.output, *dirpath.split(os.path.sep)[1:])
        file_paths = [join(dirpath, x) for x in file_names]
        good_files = _get_good_files(file_paths)
        if isdir(out_path): 
            raise OSError(
                "output directory {} already exists, "
                " refusing overwrite".format(out_path))
        os.makedirs(out_path)
        _dash_hadd(good_files, out_path, fast=config.fast, 
                   aggressive=config.aggressive)

def _dash_hadd(good_files, output, fast=False, aggressive=False): 
    def key_from_name(fname): 
        return splitext(basename(fname))[0].split('-')[0]
    if not isdir(output): 
        os.mkdir(output)
    base_keys = {key_from_name(f) for f in good_files}
    for key in base_keys: 
        out_path = join(output, '{}.h5'.format(key))
        print 'making {}'.format(out_path)
        file_group = [f for f in good_files if key in f]
        missing = _get_missing_subfiles(file_group)
        if missing: 
            subfiles_str = ', '.join(str(x) for x in sorted(missing))

            prob = "file {} can't be created, missing subfiles: {}".format(
                out_path, subfiles_str)
            if aggressive: 
                warnings.warn(prob, stacklevel=2)
            else: 
                raise IOError(prob)
        _hadd(file_group, out_path, fast=fast)
    

def _get_missing_subfiles(file_group): 
    """
    checks the 'XofY' string on the end of histogram files. 
    """
    if not 'of' in file_group[0].split('-')[-1]: 
        return []
    else: 
        extensions = [f.split('-')[-1] for f in file_group]
        numbers = set()
        total_set = set()
        for ext in extensions: 
            num, tot = splitext(ext)[0].split('of')
            numbers.add(int(num))
            total_set.add(int(tot))
        total = int(next(iter(total_set)))
        if not len(total_set) == 1: 
            gname = file_group[0].split('-')[0]
            raise IOError('two totals ({}) found for {}'.format(
                    ', '.join(total_set), gname))
        if not len(numbers) == total: 
            return set(xrange(1, total + 1)) - numbers
        return []

def _hadd(good_files, output, weights_dict={}, fast=False):
    with h5py.File(good_files[0]) as base_h5:
        weight = weights_dict.get(good_files[0],1.0)
        hadder = HistAdder(base_h5, weight=weight, wt2_ext='Wt2')
        # TODO: move these counters into a class
        sum_wt = base_h5.attrs.get('total_event_weight', 0)
        total_evt = base_h5.attrs.get('total_events', 0)
    for add_file in good_files[1:]: 
        if not isfile(add_file): 
            raise IOError("{} doesn't exist".format(add_file))
        if fast: 
            weight = weights_dict.get(add_file, None)
            with h5py.File(add_file) as add_h5: 
                hadder.fast_add(add_h5, weight=weight)
                sum_wt += add_h5.attrs.get('total_event_weight', 0)
                total_evt += add_h5.attrs.get('total_events', 0)
        else: 
            weight = weights_dict.get(add_file, 1.0)
            with h5py.File(add_file) as add_h5: 
                hadder.add(add_h5, weight=weight)
                sum_wt += add_h5.attrs.get('total_event_weight', 0)
                total_evt += add_h5.attrs.get('total_events', 0)
    if output: 
        with h5py.File(output,'w') as out_file: 
            hadder.write_to(out_file)
            out_file.attrs['total_events'] = total_evt
            if sum_wt: 
                out_file.attrs['total_event_weight'] = sum_wt
    else:
        hadder.dump()

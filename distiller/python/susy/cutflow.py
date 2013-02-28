import warnings
from os.path import isfile, isdir, expanduser, expandvars

class CorruptedCutflow(list): 
    def __init__(self, cutlist, files_used): 
        super(CorruptedCutflow,self).__init__(cutlist)
        self.files_used = files_used


def cutflow(input_files, flags, grl='', output_ntuple='', 
            btag_cal_file='', btag_cal_dir='', systematic='NONE'): 
    """
    Returns a list of pairs: (cut_name, n_passing). If output_ntuple is 
    given will also write an ntuple. 

    Flags: 
        a: aggressive --- remove bad files and retry
        b: debug susytools (don't pipe output to /dev/null)
        c: save JetFitterCharm weights
        d: is data
        e: save all events (don't do further skimming)
        f: is atlfast
        g: get branches as textfile
        i: get sparticle id (requires truth)
        m: save mv3 weights
        p: use low pt jets
        r: save c-tag ratios
        v: verbose


    This is a python-level interface for the compiled cutflow routine. 
    """
    if isinstance(input_files, str): 
        input_files = [input_files]

    assert isinstance(flags, str)
    assert isinstance(grl, str)
    assert isinstance(btag_cal_dir, str)
    assert isinstance(btag_cal_file, str)
    assert isinstance(systematic, str)

    if not input_files: 
        raise IOError("can't run cutflow, input files don't exist")

    grl = _get_fixed_pathname(grl) 
    btag_cal_dir = _get_fixed_pathname(btag_cal_dir)
    btag_cal_file = _get_fixed_pathname(btag_cal_file)
                
    input_dict = { 
        'grl': grl, 
        'trigger': 'EF_xe80_tclcw_tight', 
        'btag_cal_dir':btag_cal_dir, 
        'btag_cal_file':btag_cal_file, 
        'systematic':systematic, 
        }

    try: 
        import _distiller 
        cut_out = _distiller._distiller(
            input_files, input_dict, flags, output_ntuple)
    except RuntimeError as er: 
        if 'a' in flags and 'bad file:' in str(er): 
            bad_file = str(er).split(':')[-1].strip()
            remaining_files = [f for f in input_files if f != bad_file]
            if remaining_files:
                warnings.warn('removed {}, retrying'.format(bad_file))
                
                cutlist = cutflow(remaining_files, flags, 
                                  grl, output_ntuple, 
                                  btag_cal_file=btag_cal_file, 
                                  btag_cal_dir=btag_cal_dir)
                return CorruptedCutflow(cutlist, remaining_files)
            else: 
                warnings.warn('removed last file in ds: {}'.format(
                        input_files))
        raise 

    return cut_out

def _get_fixed_pathname(orig_path):
    if not orig_path: 
        return ''
    expanded = expandvars(expanduser(orig_path))
    if isdir(expanded): 
        if not expanded.endswith('/'): 
            expanded += '/'
    elif not isfile(expanded): 
        raise IOError("requested file '{}' can't be found".format(expanded))
    return expanded

        
def make_perf_ntuple(input_file, flags, output_ntuple=''): 
    """
    makes a performance ntuple from input_file, supposed to be compatible
    with JETNET training / testing stuff
    """
    _cutflow._make_perf_ntuple(input_file, flags, output_ntuple)
                              

def print_raw_cutflow(input_file): 
    """
    prints the raw cutflow, descending in n passing cut
    """
    cut_counts = cutflow(input_file, 'vs')
    sorted_counts = sorted(cut_counts, key=lambda cut: cut[1], reverse=True)
    for name, count in sorted_counts: 
        print '{:>20}: {}'.format(name,count)

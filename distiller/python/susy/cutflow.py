import _cutflow 
import warnings
from os.path import isfile

class CorruptedCutflow(list): 
    def __init__(self, cutlist, files_used): 
        super(CorruptedCutflow,self).__init__(cutlist)
        self.files_used = files_used


def cutflow(input_files, run_number, flags, grl='', output_ntuple=''): 
    """
    Returns a list of pairs: (cut_name, n_passing). If output_ntuple is 
    given will also write an ntuple. 

    Flags: 
        v: verbose
        d: is data
        s: is signal
        f: is atlfast
        r: save c-tag ratios
        p: use low pt jets
        b: debug susytools (don't pipe output to /dev/null)
        c: save JetFitterCharm weights
        m: save mv3 weights
        i: get sparticle id (requires truth)

        a: aggressive --- remove bad files and retry

    This is a python-level interface for the compiled cutflow routine. 
    """
    if isinstance(input_files, str): 
        input_files = [input_files]

    if not input_files: 
        raise IOError("can't run cutflow, input files don't exist")

    if grl and not isfile(grl): 
        raise IOError('grl {} not found'.format(grl))

    input_dict = { 
        'run_number':run_number, 
        'grl': grl, 
        'trigger': 'EF_xe80_tclcw_loose', 
        # 'trigger': 1, 
        'btag_cal_dir':'', 
        'btag_cal_file':'', 
        }

    try: 
        cut_out = _cutflow._cutflow(
            input_files, input_dict, flags, output_ntuple)
    except RuntimeError as er: 
        if 'a' in flags and 'bad file:' in str(er): 
            bad_file = str(er).split(':')[-1].strip()
            remaining_files = [f for f in input_files if f != bad_file]
            if remaining_files:
                warnings.warn('removed {}, retrying'.format(bad_file))
                
                cutlist = cutflow(remaining_files, run_number, flags, 
                                  grl, output_ntuple)
                return CorruptedCutflow(cutlist, remaining_files)
        raise RuntimeError('{} in cutflow'.format(str(er)))

    return cut_out

        
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
    cut_counts = cutflow(input_file, run_number, 'vs')
    sorted_counts = sorted(cut_counts, key=lambda cut: cut[1], reverse=True)
    for name, count in sorted_counts: 
        print '{:>20}: {}'.format(name,count)

import _cutflow 
import warnings

def cutflow(input_files, run_number, flags, output_ntuple = ''): 
    """
    Returns a list of pairs: (cut_name, n_passing). If output_ntuple is 
    given will also write an ntuple. 

    Flags: 
        v: verbose
        d: is data
        s: is signal
        f: is atlfast
        r: save c-tag ratios
        w: save c-tag flavor weights
        p: use low pt jets
        d: debug susytools (don't pipe output to /dev/null)

        a: aggressive --- remove bad files and retry

    This is a python-level interface for the compiled cutflow routine. 
    """
    if isinstance(input_files, str): 
        input_files = [input_files]

    if not input_files: 
        raise IOError("can't run cutflow, input files don't exist")

    try: 
        cut_out = _cutflow._cutflow(input_files, run_number, flags, 
                                    output_ntuple)
    except RuntimeError as er: 
        if 'a' in flags and 'bad file:' in str(er): 
            bad_file = str(er).split(':')[-1].strip()
            remaining_files = [f for f in input_files if f != bad_file]
            if remaining_files:
                warnings.warn('removed {}, retrying'.format(bad_file))
                return cutflow(remaining_files, run_number, flags, 
                               output_ntuple)
        raise RuntimeError('{} in cutflow'.format(str(er)))

    return cut_out


        
                
                              

def print_raw_cutflow(input_file): 
    """
    prints the raw cutflow, descending in n passing cut
    """
    cut_counts = cutflow(input_file, run_number, 'vs')
    sorted_counts = sorted(cut_counts, key=lambda cut: cut[1], reverse=True)
    for name, count in sorted_counts: 
        print '{:>20}: {}'.format(name,count)

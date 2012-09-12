import _cutflow as cf
import warnings


class NormedCutflow(object): 
    def __init__(self, norm_file): 
        self._norm_file_name = norm_file
        with open(norm_file) as txt: 
            self._norm_dict = {n: (x,e) for n,x,e in self._tup_itr(txt)}

    def _tup_itr(self,txt_file): 
        for line in txt_file: 
            line = line.split('#')[0].strip()
            if not line: 
                continue
            spl = line.split()
            short_name = spl[0]
            xsec = float(spl[2])
            evts = int(spl[4])
            yield short_name, xsec, evts

    def get_normed_counts(self, file_name): 
        
        if not file_name in self._norm_dict: 
            raise LookupError('no {} found in {}'.format(
                    file_name, self._norm_file_name))

        cut_counts = cf._cutflow(input_file, 'v')
        
                              

def print_raw_cutflow(input_file): 
    cut_counts = cf._cutflow(input_file, 'v')
    sorted_counts = sorted(cut_counts, key=lambda cut: cut[1], reverse=True)
    for name, count in sorted_counts: 
        print '{:>20}: {}'.format(name,count)

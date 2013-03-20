from stop.style import texify_sr, cr_sort

class TransferTexPrinter(object): 
    """
    Expects to be initialized by a dict with hierarchy: 
        - Control Region 
        - Signal Region
        - Transfer Factor object
    """
    def __init__(self, tf_table): 
        self.tf_table = tf_table
        control_regions = set(tf_table.keys())
        signal_regions = set()
        for cr in control_regions: 
            signal_regions |= set(tf_table[cr].keys())
        
        self.control_regions = sorted(control_regions, key=cr_sort)
        self.signal_regions = sorted(signal_regions, key=cr_sort)

    def _begin(self, n_cols): 
        return r'\begin{{tabular}}{{{}}}'.format(
            '|'.join(['c']*n_cols)) + '\n'
    def _begin_matrix(self, n_cols): 
        colstring = 'c|' + 'c'*(n_cols - 1)
        return r'\begin{{tabular}}{{{}}}'.format(colstring) + '\n'

    def _head_row(self, entries, nhline=1): 
        line = r'{}\\{}'.format(' & '.join(entries), r'\hline'*nhline) 
        return line + '\n'

    def _row_with_err(self, row_entries, prec=1, first=None): 
        if first: 
            texified_row = [first]
        else: 
            texified_row = []
        def texify(value, error): 
            return '${v:.{p}f} \pm {e:.{p}f}$'.format(
                v=value, p=prec, e=error)
        texified_row += [texify(v,e) for v,e in row_entries]
        return r'{} \\'.format(' & '.join( texified_row)) + '\n'
                
    def _end(self): 
        return r'\end{tabular}' + '\n'

class TransferFactorPrinter(TransferTexPrinter): 
    def __init__(self, tf_table): 
        super(TransferFactorPrinter,self).__init__(tf_table)
        
    def write(self, file_object): 
        n_cols = len(self.signal_regions) + 1
        file_object.write(self._begin_matrix(n_cols))
        file_object.write(self._head_row([''] + self.signal_regions))
        for cr in self.control_regions: 
            row = []
            for sr in self.signal_regions: 
                tf = self.tf_table[cr][sr]
                row.append((tf.value, tf.error))
            file_object.write(self._row_with_err(row, first=texify_sr(cr)))
        file_object.write(self._end())
    

from stop.style import texify_sr, cr_sort
from math import log, floor

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
        self.red_threshold = 0.5
        self.green_threshold = 0.2

    def _begin(self, n_cols): 
        return r'\begin{{tabular}}{{{}}}'.format(
            '|'.join(['c']*n_cols)) + '\n'
    def _begin_matrix(self, n_cols): 
        colstring = 'c|' + 'c'*(n_cols - 1)
        return r'\begin{{tabular}}{{{}}}'.format(colstring) + '\n'

    def _head_row(self, entries, nhline=1): 
        line = r'{}\\{}'.format(' & '.join(entries), r'\hline'*nhline) 
        return line + '\n'

    def _title_row(self, title, n_cols): 
        out = [r'\hline']
        out.append(r'\multicolumn{{{}}}{{{}}}{{{}}}\\ \hline'.format(
                n_cols, 'c', title))
        return '\n'.join(out + [''])

    def _row(self, row_entries, first=None): 
        if first: 
            texified_row = [first]
        else: 
            texified_row = []
        texified_row += row_entries
        return r'{} \\'.format(' & '.join( texified_row)) + '\n'

    def _row_with_err(self, row_entries, prec='auto', first=None): 
        if first: 
            texified_row = [first]
        else: 
            texified_row = []
        def texify(value, error, prec=prec): 
            if prec == 'auto': 
                try: 
                    prec = int( 1 - floor(log(error,10)) )
                except ValueError: 
                    return 'Nan'
            if prec >= 0: 
                num =  '${v:.{p}f} \pm {e:.{p}f}$'.format(
                    v=value, p=prec, e=error)
            else: 
                num = '${v:.{p}f} \pm {e:.{p}f}$'.format(
                    v=value, p=0, e=error)
            if error / value > self.red_threshold: 
                return r'\textcolor{{red}}{{{}}}'.format(num)
            elif error / value < self.green_threshold: 
                return r'\textcolor{{blue}}{{{}}}'.format(num)
            else: 
                return num
        texified_row += [texify(v,e) for v,e in row_entries]
        return r'{} \\'.format(' & '.join( texified_row)) + '\n'
                
    def _end(self): 
        return r'\end{tabular}' + '\n'

class TransferFactorTable(TransferTexPrinter): 
    def __init__(self, tf_table): 
        super(TransferFactorTable,self).__init__(tf_table)
        
    def write(self, file_object): 
        n_cols = len(self.signal_regions) + 1
        file_object.write(self._begin_matrix(n_cols))
        corner_info = (
            r'\textcolor{{red}}{{{:.1f}}}'
            r' \textcolor{{blue}}{{{:.1f}}}'.format(
                self.red_threshold, self.green_threshold))
        file_object.write(
            self._head_row([corner_info] + self.signal_regions))
        for cr in self.control_regions: 
            row = []
            for sr in self.signal_regions: 
                tf = self.tf_table[cr][sr]
                row.append((tf.value, tf.error))
            file_object.write(self._row_with_err(row, first=texify_sr(cr)))
        file_object.write(self._end())

class TransferFactorRelitiveErrorTable(TransferTexPrinter): 
    def __init__(self, tf_table): 
        super(TransferFactorRelitiveErrorTable,self).__init__(tf_table)
        ex_sr = self.signal_regions[0]
        ex_cr = self.control_regions[0]
        systematics = self.tf_table[ex_cr][ex_sr].transfer_rel_errors
        self.systematics = sorted(systematics.keys())
        self.min_to_mark = 0.5
        self.colorer = r'\textcolor{{red}}{{{}}}'

    def _mark_leading_in_tf(self,tf): 
        tf.marked_rel_errors = set()
        max_rel_error = max(abs(x) for x in tf.transfer_rel_errors.values())
        for tf_name, tf_value in tf.transfer_rel_errors.iteritems(): 
            if abs(tf_value) > self.min_to_mark * max_rel_error: 
                tf.marked_rel_errors.add(tf_name)

    def _mark_leading(self): 
        for sr in self.signal_regions: 
            for cr in self.control_regions: 
                self._mark_leading_in_tf(self.tf_table[cr][sr])
        
    def _add_cr(self, cr, file_object): 
        n_cols = len(self.signal_regions) + 1
        file_object.write(self._title_row(texify_sr(cr), n_cols))
        hint = self.colorer.format(str(self.min_to_mark) + ' / max')
        file_object.write(self._head_row(
                [hint] + self.signal_regions))
        for rel_error in self.systematics: 
            row = []
            for sr in self.signal_regions: 
                systs = self.tf_table[cr][sr].transfer_rel_errors
                string = '{:.2}'.format(systs[rel_error])
                if rel_error in self.tf_table[cr][sr].marked_rel_errors: 
                    row.append(self.colorer.format(string))
                else: 
                    row.append(string)
            line = self._row(row, first=rel_error)
            file_object.write(line)

    def write(self, file_object, control_regions='all'): 
        self._mark_leading()
        n_cols = len(self.signal_regions) + 1
        file_object.write(self._begin_matrix(n_cols))
        if control_regions == 'all': 
            control_regions == self.control_regions
        for cr in control_regions: 
            self._add_cr(cr, file_object)
        file_object.write(self._end())
        

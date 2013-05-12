import tempfile
import os, sys

class OutputFilter(object): 
    """
    Workaround filter for annoying and worthless ROOT errors. 
    """
    def __init__(self, veto_words={'TClassTable'}, accept_words={}): 
        self.veto_words = set(veto_words)
        self.accept_words = set(accept_words)
        self.temp = tempfile.NamedTemporaryFile()
    def __enter__(self): 
        sys.stdout.flush()
        sys.stderr.flush()
        self.old_out, self.old_err = os.dup(1), os.dup(2)
        os.dup2(self.temp.fileno(), 1)
        os.dup2(self.temp.fileno(), 2)
    def __exit__(self, exe_type, exe_val, tb): 
        sys.stdout.flush()
        sys.stderr.flush()
        os.dup2(self.old_out, 1)
        os.dup2(self.old_err, 2)
        self.temp.seek(0)
        for line in self.temp: 
            veto = set(line.split()) & self.veto_words
            accept = set(line.split()) & self.accept_words
            if not veto and accept: 
                sys.stderr.write(line)

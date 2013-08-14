import tempfile
import os, sys, re, errno
from time import time

def make_dir_if_none(hists_dir): 
    """
    Avoids race condition from launching multiple jobs. 
    """
    try: 
        os.makedirs(hists_dir)
    except OSError as err: 
        if err.errno == errno.EEXIST and os.path.isdir(hists_dir): 
            pass
        else: 
            raise

class ProgressMeter(object): 
    def __init__(self, level=3): 
        self.last_epoch = None
        self.level = level
        print 'walking'
        self.n_query = 0
        self.start = time()

    def get_walk_progress(self,root): 
        self.n_query += 1
        level = self.level
        dir_path = root.split('/')
        if len(dir_path) >= level:
            if dir_path[-level] != self.last_epoch: 
                self.last_epoch = dir_path[-level]
                rate = self.n_query / float(time() - self.start) 
                self.start = time()
                self.n_query = 0
                print 'working on {} ({:.1f} Hz)'.format(
                    dir_path[-level], rate)


class OutputFilter(object): 
    """
    Workaround filter for annoying and worthless ROOT errors. 
    """
    def __init__(self, veto_words={'TClassTable'}, accept_words={}, 
                 accept_re=''): 
        self.veto_words = set(veto_words)
        self.accept_words = set(accept_words)
        self.temp = tempfile.NamedTemporaryFile()
        if accept_re: 
            self.re = re.compile(accept_re)
        else: 
            self.re = None
    def __enter__(self): 
        self.old_out, self.old_err = os.dup(1), os.dup(2)
        os.dup2(self.temp.fileno(), 1)
        os.dup2(self.temp.fileno(), 2)
    def __exit__(self, exe_type, exe_val, tb): 
        sys.stdout.flush()
        sys.stderr.flush()
        os.dup2(self.old_out, 1)
        os.dup2(self.old_err, 2)
        os.close(self.old_out)
        os.close(self.old_err)
        self.temp.seek(0)
        for line in self.temp: 
            veto = set(line.split()) & self.veto_words
            accept = set(line.split()) & self.accept_words
            if self.re is not None: 
                re_found = self.re.search(line)
            else: 
                re_found = False
            if not veto and (accept or re_found): 
                sys.stderr.write(line)

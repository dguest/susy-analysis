import tempfile
import os, sys, re, errno
from time import time
from itertools import product
import math

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

def helvetify():
    """
    Load 'Helvetica' default font (may be Arial for now)
    """
    from matplotlib import rc
    # 'Comic Sans MS', 'Trebuchet MS' works, Arial works in png...
    rc('font',**{'family':'sans-serif','sans-serif':['Arial']})

def fast_walk(base_dir):
    """
    trying to be clever about using os.walk (which is slow on omega for
    some reason). Assumes that directory structures are hypercubes, and
    that files only exist in the leaf nodes, and that we only care about
    files...
    """
    dirs_list = [ [base_dir] ]
    files_list = [ [] ]
    for root, subdirs, files in os.walk(base_dir):
        dirs_list.append(subdirs[:])
        files_list.append(files[:])
        if subdirs:
            subdirs[:] = [subdirs[0]]

    for dirs in dirs_list:
        dirs.sort()

    for dir_tup in product(*dirs_list[:-1]):
        root = os.path.join(*dir_tup)
        yield root, [], files_list[-1]


class ProgressMeter(object):
    def __init__(self, level=3):
        self.last_epoch = None
        self.level = level
        print('walking')
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
                print('working on {} ({:.1f} Hz)'.format(
                    dir_path[-level], rate))

class FlatProgressMeter(object):
    _stat_line = '\r{n:{l}} of {t} ({p:4.0%}, {hz:5.1f} {u})'
    _units = {-1:'mHz',0:'Hz ',1:'kHz',2:'MHz',3:'GHz'}
    def __init__(self, total, update_percent=1):
        self.total = total
        self.update_inc = max(total * update_percent / 100, 1)
        self.last_time = time()
        self.last_count = 0
        self.tot_len = len(str(total))
    def update(self, count):
        cp = count + 1 # we start with 1
        if cp % self.update_inc == 0:
            now = time()
            hz = float(cp - self.last_count) / (now - self.last_time)
            self.last_count = cp
            self.last_time = now
            order = 0 if hz == 0 else int(math.floor(math.log(hz,1e3)))
            try:
                unit = self._units[order]
            except IndexError:
                order = 0
                unit = 'Hz '
            vals = dict(
                n=cp, l=self.tot_len, t=self.total,
                p=float(cp)/self.total, hz=hz/1000**order, u=unit)
            sys.stdout.write(self._stat_line.format(**vals))
            sys.stdout.flush()
        if cp == self.total:
            sys.stdout.write('\n')

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

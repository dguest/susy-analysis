#!/usr/bin/env python3
"""
Dump some defs as twikitable
"""
from scharm.limits.limitsty import alpha_names as alp
import re, math

def twikify(string):
    new = ''
    is_open = True
    for idx, char in enumerate(string):
        if char == '$':
            new += '%$' if is_open else '$%'
            is_open = not is_open
        else:
            new += char
    return new

def get_pairs():
    skeys = sorted(alp)

    pairs = []
    for one in skeys:
        translated = alp[one]
        repd = twikify(translated)
        pairs.append( (one, repd))
    return pairs

if __name__ == '__main__':

    pairs = get_pairs()
    cols = 4
    el = ['']
    print(' | '.join(el + ['*int*','*disp*', '']*cols + el))
    nrow = math.ceil(len(pairs) / cols)
    for row in range(nrow):
        vals = ['']
        for col in range(cols):
            try:
                pair = pairs[row + col*nrow]
            except IndexError:
                pair = ['']*2
            vals += pair
            vals += el
        vals += el
        print(' | '.join(vals))

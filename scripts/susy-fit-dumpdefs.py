#!/usr/bin/env python3.3
"""
Dump some defs as twikitable
"""
from scharm.limits.limitsty import alpha_names as alp
import re

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

if __name__ == '__main__':
    # twikify = re.compile('\$(.*)\$')

    skeys = sorted(alp)
    for one in skeys:
        translated = alp[one]
        repd = twikify(translated)
        print('|', one, '|', repd, '|')

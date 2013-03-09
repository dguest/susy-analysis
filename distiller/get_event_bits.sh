#!/usr/bin/env bash

sed -n -r 's/^ *const ull_t ([^ ]*) *=([^;]*);/\(\"\1\", \2\),/gp'\
 include/EventBits.hh | sed 's/1u/1/g' 
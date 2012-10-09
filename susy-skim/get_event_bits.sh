#!/usr/bin/env bash

sed -n -r 's/^ *const unsigned ([^ ]*) *=([^;]*);/\1 = \2/gp'\
 include/EventBits.hh | sed 's/1u/1/g' 
#!/usr/bin/env bash
set -e
gcc -std=c11 -pedantic -Wall -Wextra -Werror main.c -o exec;
in="$1.in"
out="$1.out"
< $in ./exec | diff - $out


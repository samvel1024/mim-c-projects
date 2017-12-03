#!/usr/bin/env bash
set -e
in="./tests/$1.in"
out="./tests/$1.out"
< ${in} ./exec > "_temp.out";
diff "_temp.out" ${out};
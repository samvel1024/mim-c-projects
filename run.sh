#!/usr/bin/env bash
set -e
./compile.sh
cat $1 | ./exec
#!/usr/bin/env bash
set -e
./compile.sh
./diff.sh $1

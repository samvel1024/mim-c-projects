#!/usr/bin/env bash
set -e
./compile.sh
ls ./tests | grep .in | while read -r line ; do
    name=${line%.*}
    echo "Running ${name}..."
    ./diff.sh "${name}"
    echo "OK"
done

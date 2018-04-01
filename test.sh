#!/bin/bash
set -e

prog=$1
dir=$2

if [ ! -f "$prog" ]; then
    echo "Executable file not found!"
    exit 1
fi

if [ ! -d "$dir" ]; then
  echo "Could not find test directory!"
  exit 1
fi

cd ${dir}

for f in ./*.in; do
  test=${f%.*}
  expect_stderr="${test}.err"
  expect_stdout="${test}.out"
  printf "Test ${test} "
  cat ${f} | ../${prog} 2> ../temperr 1> ../tempout
  diff "../_temp_stdout" ${expect_stdout}
  diff "../_temp_stderr" ${expect_stderr}
  echo "OK"
done

#If there are no errors remove temporary files
rm -rf "../_temp_stderr"
rm -rf "../_temp_stdout"
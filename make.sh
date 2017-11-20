#!/usr/bin/env bash
gcc -std=c11 -pedantic -Wall -Wextra -Werror main.c -o exec;
./exec < input.txt;
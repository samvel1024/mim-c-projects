#!/usr/bin/env bash
set -e
gcc -std=c11 -pedantic -Wall -Wextra -Werror main.c -o exec;
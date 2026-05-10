#! /usr/bin/env bash
set -e
cmake -B build
cmake --build build

ctest --test-dir build/sysltree --output-on-failure
ctest --test-dir build/symbol --output-on-failure

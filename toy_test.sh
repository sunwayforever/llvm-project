#!/bin/bash
clang toy_test/test.c -c -emit-llvm -O0 -o /tmp/test.bc
llvm-dis /tmp/test.bc
cat /tmp/test.ll
./build/bin/llc /tmp/test.bc -march=toy --debug -o -

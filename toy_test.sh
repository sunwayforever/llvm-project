#!/bin/bash
rm /tmp/test.{bc,s,o,ll} &>/dev/null
rm /tmp/a.out &>/dev/null

clang toy_test/test.c -c -emit-llvm -O0 -o /tmp/test.bc && \
llvm-dis /tmp/test.bc && \
cat /tmp/test.ll && \
(./build/bin/llc /tmp/test.bc -march=toy --debug -o - |tee /tmp/test.s) &&
riscv64-linux-gnu-gcc -march=rv32g -mabi=ilp32 -c -O0 /tmp/test.s -o /tmp/test.o && \
riscv64-linux-gnu-objdump -d /tmp/test.o 
# riscv64-linux-gnu-gcc toy_test/main.c /tmp/test.s -O0 -o /tmp/a.out -static && \
# qemu-riscv64 /tmp/a.out

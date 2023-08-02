// REQUIRES: system-linux
// RUN: clang -o %t %s
// RUN: llvm-mctoll -d -I /usr/include/stdio.h %t
// RUN: clang -o %t1 %t-dis.ll
// RUN: %t1 2>&1 | FileCheck %s
// CHECK: Hello world!

#include <stdio.h>
int main(int argc, char **argv) {
  int a = argc;
  int b = argc + 1;
  return b - 2;
}

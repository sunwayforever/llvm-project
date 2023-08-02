// 2023-06-02 14:04
int bar(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7, int b) {
  return a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7 + b;
}

int foo() {
  int ret = bar(0, 1, 2, 3, 4, 5, 6, 7, 8);
  return ret;
}

#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  assert(foo() == 36);
  return 0;
}

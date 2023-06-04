// 2023-06-02 14:04
int bar(int a, int b, int c, int d, int e) { return a + b + c + d + e; }
int foo() {
  int ret = bar(1, 2, 3, 4, 5);
  return ret;
}

#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  assert(foo() == 15);
  return 0;
}

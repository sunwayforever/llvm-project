// 2023-05-31 18:12
int bar(int a) { return a + 1; }
int foo(int a) {
  int x = bar(a);
  return x + 1;
}

#include <assert.h>

int main(int argc, char *argv[]) {
  int x = foo(1);
  assert(x == 3);
}

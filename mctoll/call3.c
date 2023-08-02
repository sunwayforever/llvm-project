// 2023-06-04 10:12
void bar(int a, int b, int c) {}
int foo(int a, int b) {
  bar(1, 2, 3);
  return 1;
}
#include <assert.h>

int main(int argc, char *argv[]) {
  assert(foo(1, 2) == 1);
  return 0;
}

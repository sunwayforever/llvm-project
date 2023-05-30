// 2023-05-26 15:44
#include <assert.h>

extern int a, b, c, d, e, f;
extern void foo();
int main(int argc, char *argv[]) {
  foo();
  assert(a == 0);
  assert(b == 1);
  assert(c == 0);
  assert(d == 1);
  assert(e == 0);
  assert(f == 1);
}

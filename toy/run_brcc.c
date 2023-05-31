// 2023-05-26 15:44
#include <assert.h>

extern int a, b, c, d, e, f;
extern void foo();
int main(int argc, char *argv[]) {
  a = 1;
  b = 0;
  c = 1;
  d = 1;
  e = 0;
  f = 0;
  foo();
  assert(a == 2);
  assert(b == 1);
  assert(c == 2);
  assert(d == 2);
  assert(e == 0);
  assert(f == 1);
}

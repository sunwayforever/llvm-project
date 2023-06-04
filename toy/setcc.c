// 2023-05-31 18:18
int a, b, c, d, e, f;

void foo() {
  int x = 0;
  a = x > 10;
  b = x < 10;
  c = x == 10;
  d = x != 10;
  e = (x >= 1);
  f = (x <= 0);
}

#include <assert.h>

int main(int argc, char *argv[]) {
  foo();
  assert(a == 0);
  assert(b == 1);
  assert(c == 0);
  assert(d == 1);
  assert(e == 0);
  assert(f == 1);
}

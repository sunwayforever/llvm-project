int a, b, c, d, e, f;

void foo() {
  if (a >= 1) {
    a += 1;
  }
  if (d <= 1) {
    d += 1;
  }
  if (b == 0) {
    b += 1;
  }
  if (c != 0) {
    c += 1;
  }
  if (e > 1) {
    e += 1;
  }
  if (f < 1) {
    f += 1;
  }
};

#include <assert.h>

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

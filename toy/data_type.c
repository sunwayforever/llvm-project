// 2023-06-01 10:44
char a, c;
short b, d;
int e, f, g;

char foo(short x) {
  a = 1;
  c = a;
  b = x;
  d = b;
  e = 1;
  f = e;
  g = a;
  return a;
}

#include <assert.h>

int main(int argc, char *argv[]) {
  char x = foo(1);
  assert(a == 1);
  assert(c == 1);
  assert(b == 1);
  assert(d == 1);
  assert(e == 1);
  assert(f == 1);
  assert(g == 1);
}

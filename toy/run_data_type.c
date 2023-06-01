// 2023-06-01 14:01
#include <assert.h>

extern char a, c;
extern short b, d;
extern int e, f, g;

extern char foo(short x);
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

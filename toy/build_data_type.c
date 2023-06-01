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

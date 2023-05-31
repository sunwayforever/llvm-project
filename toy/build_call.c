// 2023-05-31 18:12
int bar(int a) { return a + 1; }
int foo(int a) {
  int x = bar(a);
  return x + 1;
}

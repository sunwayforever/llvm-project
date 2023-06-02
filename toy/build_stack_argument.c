// 2023-06-02 14:04
int bar(int a, int b, int c, int d, int e) { return a + b + c + d + e; }
int foo() {
  int ret = bar(1, 2, 3, 4, 5);
  return ret;
}

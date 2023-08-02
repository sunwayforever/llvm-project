// 2023-05-31 18:20
void foo() {
  int a = 10;
  int b = a + 10;
  int c = a - 1;
  int d = b + c;
  int e = b * c;
  int f = b / c;
  int g = b % c;
  int h = b & 1;
  int i = b | 1;
  int j = b & c & d;
  int k = b | c | 1;
};

int main(int argc, char *argv[]) {
  foo();
  return 0;
}

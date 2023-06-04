// 2023-05-31 18:14
int x[2];
void foo() {
  int y[2];
  y[1] = 1;
  x[0] = y[1];
  x[1] = 2;
};

#include <assert.h>
int main(int argc, char *argv[]) {
  foo();
  assert(x[0] == 1);
  assert(x[1] == 2);
}

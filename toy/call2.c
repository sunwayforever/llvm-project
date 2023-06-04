// 2023-06-02 17:30
int bar() { return 1; }
int foo() { return bar(); }

#include <assert.h>

int main(int argc, char *argv[]) {
  assert(foo() == 1);
  return 0;
}

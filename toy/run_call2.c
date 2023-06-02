// 2023-06-03 19:58
#include <assert.h>

extern int foo();

int main(int argc, char *argv[]) {
  assert(foo() == 1);
  return 0;
}

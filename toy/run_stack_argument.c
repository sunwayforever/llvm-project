// 2023-06-02 14:43
#include <assert.h>
#include <stdio.h>

extern int foo();
int main(int argc, char *argv[]) {
  assert(foo() == 15);
  return 0;
}

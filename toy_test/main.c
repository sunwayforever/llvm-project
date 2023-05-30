// 2023-05-26 15:44
#include <assert.h>

extern int foo(int);
int main(int argc, char *argv[]) {
  int y = foo(0);
  assert(y == 1);
}

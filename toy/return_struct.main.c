// 2023-06-08 16:59
#include "return_struct.h"
#include <assert.h>

int main(int argc, char *argv[]) {
  X x = foo();
  assert(x.a0 == 1);
  assert(x.a1 == 2);
  assert(x.a7 == 3);
  return 0;
}

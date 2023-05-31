// 2023-05-31 18:16
#include <assert.h>
extern int x[2];
extern void foo();
int main(int argc, char *argv[]) {
  foo();
  assert(x[0] == 1);
  assert(x[1] == 2);
}

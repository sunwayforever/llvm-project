// 2023-06-15 10:58
#include <assert.h>

int main(int argc, char *argv[]) {
  int x = 1 << 11;
  int y = (1 << 11) - 1;
  assert((x - y) == 1);
  x = 1 << 20;
  y = (1 << 20) - 1;
  assert((x - y) == 1);
  return 0;
}

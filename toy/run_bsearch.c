// 2023-06-01 14:55
#include <assert.h>
#include <stdio.h>

extern int bsearch(int key, int *base, int n);

int x[] = {1, 2, 3, 4, 5, 6, 7, 8};

int main(int argc, char *argv[]) {
  assert(bsearch(7, x, sizeof(x) / sizeof(x[0])) == 6);
  assert(bsearch(9, x, sizeof(x) / sizeof(x[0])) == -1);
  return 0;
}

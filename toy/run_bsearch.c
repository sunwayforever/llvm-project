// 2023-06-01 14:55
#include <stdio.h>

extern int bsearch(int key, int *base, int n);
int main(int argc, char *argv[]) {
  int x[] = {1, 2, 3, 4, 5, 6, 7, 8};
  printf("%d\n", bsearch(5, x, sizeof(x) / sizeof(x[0])));
  return 0;
}

// 2023-06-01 14:43
int bsearch(int key, int *base, int n) {
  int lo = 0;
  int hi = n;
  while (lo < hi) {
    int mid = (lo + hi) / 2;
    if (base[mid] == key) {
      return mid;
    }
    if (base[mid] < key) {
      lo = mid + 1;
    } else {
      hi = mid - 1;
    }
  }
  return -1;
}

#include <assert.h>
#include <stdio.h>

int x[] = {1, 2, 3, 4, 5, 6, 7, 8};

int main(int argc, char *argv[]) {
  assert(bsearch(7, x, sizeof(x) / sizeof(x[0])) == 6);
  assert(bsearch(9, x, sizeof(x) / sizeof(x[0])) == -1);
  return 0;
}

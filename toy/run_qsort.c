// 2023-06-01 16:10
#include <assert.h>
#include <stdio.h>

extern void qsort(int *data, int lo, int hi);

int data[] = {4, 1, 2, 3, 1, 2, 1, 2};

int main(int argc, char *argv[]) {
  int N = sizeof(data) / sizeof(int);
  qsort(data, 0, N);
  for (int i = 1; i < N; i++) {
    assert(data[i] >= data[i - 1]);
  }
}

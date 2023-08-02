// 2023-06-01 15:43
#define SWAP(data, a, b)                                                       \
  do {                                                                         \
    int tmp = data[b];                                                         \
    data[b] = data[a];                                                         \
    data[a] = tmp;                                                             \
  } while (0);

void qsort(int *data, int lo, int hi) {
  if (lo >= hi) {
    return;
  }
  int pivot = lo;
  int curr = lo + 1;
  while (curr < hi) {
    if (data[curr] >= data[pivot]) {
      curr += 1;
    } else {
      SWAP(data, curr, pivot + 1);
      SWAP(data, pivot, pivot + 1);
      pivot += 1;
      curr += 1;
    }
  }
  qsort(data, lo, pivot);
  qsort(data, pivot + 1, hi);
}

#include <assert.h>
#include <stdio.h>

int data[] = {4, 1, 2, 3, 1, 2, 1, 2};

int main(int argc, char *argv[]) {
  int N = sizeof(data) / sizeof(int);
  qsort(data, 0, N);
  for (int i = 1; i < N; i++) {
    assert(data[i] >= data[i - 1]);
  }
}

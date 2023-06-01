// 2023-06-01 14:43
int bsearch(int key, int *base, int n) {
  int lo = 0;
  int hi = n;
  int mid = (lo + hi) / 2;
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

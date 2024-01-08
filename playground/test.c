// 2024-01-08 14:52
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
  int sum = 0;
  for (int i = 0; i < argc; i++) {
    sum += i;
  }
  return sum;
}

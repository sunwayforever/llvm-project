// 2023-06-04 16:52
#include <assert.h>

float foo(float a, float b) { return a + b + (double)0.1; }
int main(int argc, char *argv[]) {
  float x = foo(0.1f, 0.2f);
  assert(x == 0.4f);
  return 0;
}

// 2023-06-04 16:52
#include <assert.h>
#include <math.h>

float foo(float a, double b) {
  float x = a + b + 0.1;
  float y = x * 0.1f;
  float z = y / 2.0f;
  float k = z * 2.0f;
  float m = k * 0.1f + 0.001f;
  float n = __builtin_fmaxf(m, 0.006f);
  return n;
}
int main(int argc, char *argv[]) {
  float x = foo(0.1f, 0.2);
  assert(fabsf(x - 0.006f) < 0.001f);
  assert(fabsf(x - 0.006f) <= 0.001f);
  assert(x >= 0.001f);
  assert(x > 0.001f);
  int y = x > 0.1f;
  return 0;
}

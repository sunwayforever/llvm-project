// 2023-06-15 18:57
#include <assert.h>
#include <math.h>
#include <stdio.h>

extern void print(float x);

float Cos(float x) {
  int i, factor;
  float result, power;

  result = 1.0f;
  factor = 1;
  power = x;
  for (i = 2; i <= 10; i++) {
    factor = factor * i;
    power = power * x;
    /* NOTE: target lowering 需要 setBooleanContents(ZeroOrOneBooleanContent),
     * 否则会出错*/
    if ((i & 1) == 0) {
      if ((i & 3) == 0) {
        result = result + power / factor;
      } else {
        result = result - power / factor;
      }
    }
  }
  return (result);
}

void foo() {
  float x = Cos(0.785398f);
  assert(fabs(x - 0.707107) < 0.01);
}

int main(int argc, char *argv[]) {
  foo();
  return 0;
}

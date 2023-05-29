int x = 0;

void bar() { x++; }

void foo() {
  x++;
  bar();
}

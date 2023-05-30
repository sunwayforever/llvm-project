int x = 0;
void bar(int a) {
    x += a;
}
void foo(int a) {
    bar(a + 1);
}

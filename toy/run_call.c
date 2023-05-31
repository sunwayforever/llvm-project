// 2023-05-31 18:13
#include <assert.h>

extern int foo(int);
int main(int argc, char *argv[]) {
    int x = foo(1);
    assert(x == 3);
}

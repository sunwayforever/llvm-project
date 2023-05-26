// 2023-05-26 15:44
#include <assert.h>

extern int x;
extern void foo();
int main(int argc, char *argv[]) {
    foo();
    assert(x == 1);
}

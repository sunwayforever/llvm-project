// 2023-06-19 16:50

#include <stdio.h>

int main(int argc, char *argv[]) {
    void *x = __builtin_toy_getsp();
    printf("%p\n", x);
    return 0;
}

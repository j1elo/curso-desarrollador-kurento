// <#file>

#include <stdio.h>

int change(int a, int b)
{
    // 'a' and 'b' are value copies
    a = 20;
    return a + b;
}

int main(void)
{
    int a = 90;
    int b = 10;
    int c = change(a, b);
    printf("a: %d\n", a);
    printf("b: %d\n", b);
    printf("c: %d\n", c);

    return 0;
}

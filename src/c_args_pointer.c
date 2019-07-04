// <#file>

#include <stdio.h>

int change(int* pt_a, int b)
{
    // 'pt_a' points to a memory area
    // 'b' is a value copy
    *pt_a = 20;
    return *pt_a + b;
}

int main(void)
{
    int a = 90;
    int b = 10;
    int c = change(&a, b);
    printf("a: %d\n", a);
    printf("b: %d\n", b);
    printf("c: %d\n", c);

    return 0;
}

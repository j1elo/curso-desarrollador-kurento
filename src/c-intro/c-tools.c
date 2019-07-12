// <#file>

#include <math.h>
#include <stdio.h>

int calc(int a, int b)
{
    return a + sqrt(b);
}

int main(void)
{
    printf("printf 1, address of calc(): %p\n", calc);
    int value = calc(3, 25);
    printf("printf 2, value from calc(): %d\n", value);

    return 0;
}

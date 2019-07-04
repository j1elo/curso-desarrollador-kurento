// <#file>

#include <math.h>   // Imports function 'sqrt()'
#include <stdio.h>  // Imports function 'printf()'

int calc(int a, int b)
{
    return a + sqrt(b);
}

int main(void)
{
    int value = calc(3, 25);
    printf("value: %d\n", value);

    return 0;
}

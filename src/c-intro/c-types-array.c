// <#file>

#include <stdio.h>

int main(void)
{
    // Pointers are memory accessors
    // -----------------------------

    int number = 50;  // Type: int

    printf("number: %d\n", number);

    int* pt_number = &number;  // Type: int*

    printf("pt_number: %p\n", pt_number);

    *pt_number = 25;

    printf("number: %d\n", number);


    // Arrays are pointers with size
    // -----------------------------

    int prices[3] = {10, 20, 30};  // Type: int[3] ~ int*

    printf("prices[0]: %d\n", prices[0]);
    printf("prices[1]: %d\n", prices[1]);
    printf("prices[2]: %d\n", prices[2]);

    int* pt_prices_0 = prices;
    int* pt_prices_1 = (prices + 1);
    int* pt_prices_2 = (prices + 2);

    printf("pt_prices_0: %p\n", pt_prices_0);
    printf("pt_prices_1: %p\n", pt_prices_1);
    printf("pt_prices_2: %p\n", pt_prices_2);

    *pt_prices_0 = 111;
    *pt_prices_1 = 222;
    *pt_prices_2 = 333;

    printf("prices[0]: %d\n", prices[0]);
    printf("prices[1]: %d\n", prices[1]);
    printf("prices[2]: %d\n", prices[2]);


    // Pointer algebra is equivalent to array access
    // ----------------------------------------------

    char* letters = "abcdefg";
    char letter_e = letters[4];
    char letter_f = *(letters + 5);

    printf("letter_e: %c\n", letter_e);
    printf("letter_f: %c\n", letter_f);

    char* pt_letter = letters;

    printf("pt_letter: %c\n", *pt_letter);

    pt_letter += 1;

    printf("pt_letter: %c\n", *pt_letter);

    pt_letter += 3;

    printf("pt_letter: %c\n", *pt_letter);

    return 0;
}

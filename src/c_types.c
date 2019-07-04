// <#file>

#include <stdio.h>

int main(void)
{
    int seconds = 60;
    printf("seconds: %d, size: %zu B\n", seconds, sizeof(seconds));
    // %d: signed decimal integer

    float fps = 29.97f;
    printf("fps: %.3f, size: %zu B\n", fps, sizeof(fps));
    // %f: floating point (both 'float' and 'double')
    // .3: print only 3 digits from the fractional part

    double plank = 6.62607015E-34;
    printf("plank: %e, size: %zu B\n", plank, sizeof(plank));
    // %e: floating point in scientific notation

    struct ClockTime {
        int hours;
        int minutes;
        int seconds;
    };
    struct ClockTime clock_time = {23, 59, 59};
    printf("clock_time size: %zu B\n", sizeof(clock_time));
    // No format specifier for structs (no reflection in C)

    char vowel = 'a';
    printf("vowel: %c, size: %zu B\n", vowel, sizeof(vowel));
    // %c: character

    char* letters = "abcdefg";  // Implicit: ends with '\0'
    printf("letters: %s, size: %zu B\n", letters, sizeof(letters));
    // %s: string of characters ending with '\0'

    return 0;
}

#include<stdio.h>
#include<stdlib.h>

void
mystery (int n)
{
    n += 5;
    n /= 10;
    printf("%s\n", "**********" + 10 - n);
}

int
main (int argc, char **argv)
{
    printf("%c\n", *("xyz" + 1));   // "xyz" + 1指向y，结果输出y
    printf("%c\n", "xyz"[2]);       // "xyz"[2]指向z，结果输出z
    return EXIT_SUCCESS;
}
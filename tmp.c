#include<stdio.h>

unsigned int reverse_bits (unsigned int value)
{
    unsigned int answer;
    unsigned int i;

    answer = 0;

    /*
    ** 这样写回循环int的位数次，保证了与特定机器字长无关。value向左走，answer向右走，两者是mirror。
    */
    for (i = 1; i != 0; i <<= 1) {
        answer <<= 1;
        if (value & 1) answer |= 1;    /* 如果value最低位是1，则把answer的最低位也置1 */
        value >>= 1;
    }
    return answer;
}

int test()
{
    unsigned int value;
    unsigned int answer;

    value = 25;
    answer = 2550136832;
    if (reverse_bits(value) == 2550136832) return 1;
    else return 0;
}

int main()
{
    if (test()) printf("Success.\n");
    else printf("Failed.\n");
}
#include<stdio.h>
#include<stddef.h>   // offsetof 定义的文件

struct ALIGN
{
    char a;
    int b;
    char c;
};

struct ALIGN2
{
    int b;
    char a;
    char c;
};

int
main ()
{
    struct ALIGN align;
    struct ALIGN2 align2;
    printf("sizeof(align) = %lu\n", sizeof(align));
    printf("sizeof(align2) = %lu\n", sizeof(align2));
    printf("offset of b in ALIGN = %lu\n", offsetof(struct ALIGN, b));  // 输出4
    return 0;
}
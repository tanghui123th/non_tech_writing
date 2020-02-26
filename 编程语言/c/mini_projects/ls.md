# ls

## 一些结构体的介绍

首先是DIR

```c
struct __dirstream
{
    void *__fd;
    char *__data;
    int  __entry_data;
    char *__ptr;
    int __entry_ptr;
    size_t __allocation;
    size_t __size;
    __libc_lock_define (, __lock)
};
typedef struct __dirstream DIR;
```

dirent

```c
struct dirent
{
   long d_ino;                   /* 索引节点号 */
   off_t d_off;                  /*在目录文件中的偏移 */
   unsigned short d_reclen;      /*文件名长 */
   unsigned char d_type;         /* 文件类型 */
   char d_name [NAME_MAX+1];     /* 文件名，最长255字符 */
}
```


# pwd

## 通过系统函数getcwd实现pwd命令

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    char buf[1024];

    char *cwd = getcwd(buf, sizeof(buf));

    if (cwd == NULL) {
        perror("Get current working directory fail.\n");
        exit(-1);
    } else {
        printf("Current working directory is: %s\n", cwd);
    }
    return 0;
}
```

## Linux中的文件

我们通过一个示例程序来说明Linux中的文件系统

```c
/*
*  文件名：filestat.c
*  描述：打印指定文件名的inode信息
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>  // 是Unix/Linux系统的基本系统数据类型的头文件，含有size_t，time_t，pid_t等类型。
#include <sys/stat.h>   // 是unix/linux系统文件状态所在的头文件
#include <unistd.h>

struct stat file_stat;

void print_file_stat(struct stat *fs)
{
    printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
    printf("inode: \t\t\t\t%ld\n", fs->st_ino);
    printf("protection: \t\t\t%o\n", fs->st_mode);   // 以八进制形式输出无符号整数(不输出前缀O)
    printf("number of hard links: \t\t%lu\n", fs->st_nlink);
    printf("user ID of owner: \t\t%d\n", fs->st_uid);
    printf("group ID of owner: \t\t%d\n", fs->st_gid);
    printf("file size in bytes: \t\t%ld\n", fs->st_size);
    printf("time of last access: \t\t%s", ctime(&fs->st_atime));
    printf("time of last modification: \t%s", ctime(&fs->st_mtime));
    printf("time of last change: \t\t%s", ctime(&fs->st_ctime));
}

int main(int argc, char* argv[])
{
    if (2 != argc) {
        fprintf(stderr, "Usage: %s filename...\n", argv[0]);
        exit(-1);
    }

    // int stat(const char *file_name, struct stat *buf )
    // 成功返回0，失败返回-1
    if (0 != stat(argv[1], &file_stat)) {  
        perror("stat");
        exit(-1);
    }
    print_file_stat(&file_stat);

    return 0;
}
```

运行程序，输出

```bash
leo@ubuntu:~/c_test$ ./a.out filestat.c
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
inode: 				530528
protection: 			100664
number of hard links: 		1
user ID of owner: 		1000
group ID of owner: 		1000
file size in bytes: 		1382
time of last access: 		Tue Feb 25 00:13:18 2020
time of last modification: 	Tue Feb 25 00:12:56 2020
time of last change: 		Tue Feb 25 00:12:56 2020
```

## Linux中的目录

```c
/*
* 文件名：directorylist.c
* 描述：打印指定目录的内容列表
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>     // dirent.h是用于目录操作的头文件，POSIX标准
#include <sys/types.h>

int main(int argc, char* argv[])
{
    if (2 != argc) {
        fprintf(stderr, "Usage: %s filename...\n", argv[0]);
        exit(-1);
    }

    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    // DIR *opendir(const char *dirname);
    if (NULL == (dp = opendir(argv[1]))) {
        fprintf(stderr, "Can not open input Directory [%s]\n", argv[1]);
        exit(-1);
    }
    printf("Directory [%s] Content List:\n", argv[1]);
    while (NULL != (dptr = readdir(dp))) {
        printf("inode-number: %-10ld \t filename:%s\n", dptr->d_ino, dptr->d_name);
    }
    closedir(dp);
    return 0;
}
```

我们打印当前目录

```bash
leo@ubuntu:~/c_test$ ./a.out .
Directory [.] Content List:
inode-number: 524549     	 filename:file.in
inode-number: 525431     	 filename:a.out
inode-number: 532047     	 filename:directorylist.c
inode-number: 538140     	 filename:..
inode-number: 530528     	 filename:filestat.c
inode-number: 530063     	 filename:mypwd1.c
inode-number: 524559     	 filename:copy.c
inode-number: 525443     	 filename:file.out
inode-number: 525429     	 filename:.
```

## 实现我们自己的pwd

思路：

1. 通过特殊文件名`.`获得当前目录的inode-number
2. 通过特殊的文件名`..`获取当前目录的父级目录的inode-number
3. 判断当前目录和上级目录的inode-number是否一样
4. 如果两个inode-number一样说明达到根目录，输出完整路径，退出程序
5. 如果两个inode-number不一样，切换至父级目录，根据步骤1获取的inode-number，在父级目录中搜索对应的文件名并记录下来，然后重新回到步骤1

```c
/*
* 文件名：mypwd.c
* 描述：实现简单的pwd命令
*/
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define MAX_DIR_DEPTH (256)

/* get inode-nubmer by filename */
ino_t get_ino_by_name(char *filename)
{
    struct stat file_stat;
    if (NULL != stat(filename, &file_stat)) {
        perror("stat");
        exit(-1);
    }
    return file_stat.st_ino;
}

/* get filename in current directory by ino */
char* find_name_by_ino(ino_t ino)
{
    DIR *dp = NULL;
    struct dirent *dptr = NULL;
    char *filename = NULL;
    if (NULL == (dp = opendir("."))) {
        fprintf(stderr, "Can not open Current Directory\n");
        exit(-1);
    }
    while (NULL != (dptr = readdir(dp))) {
        if (dptr->d_ino == ino) {
            filename = strdup(dptr->d_name);  // returns a pointer to a new string which is a duplicate of the string
            break;
        }
    }
    closedir(dp);
    return filename;
}

int main(int argc, char* argv[])
{
    char *dir_stack[MAX_DIR_DEPTH];
    unsigned int current_depth = 0;

    // build dir stack, notice that sequence is reversed
    for (;;) {
        ino_t current_ino = get_ino_by_name(".");
        ino_t parent_ino = get_ino_by_name("..");

        if (current_ino == parent_ino)
            break;

        chdir("..");
        dir_stack[current_depth++] = find_name_by_ino(current_ino);
        if (current_depth >= MAX_DIR_DEPTH) {
            fprintf(stderr, "Directory tree is too deep.\n");
            exit(-1);
        }
    }

    // print pwd
    for (int i = current_depth - 1; i >= 0; i--) {
        fprintf(stdout, "/%s", dir_stack[i]);
    }
    fprintf(stdout, "%s\n", current_depth == 0 ? "/" : "");

    return 0;
}
```

输出

```bash
leo@ubuntu:~/c_test$ ./a.out 
/home/leo/c_test
```

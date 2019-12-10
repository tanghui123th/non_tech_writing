4 linux环境
===========

在多进程并发执行的情况下，为了保证程序之间不会相互干扰，了解他们的环境很重要。

4.1 程序参数
------------

getopt
~~~~~~

c程序用argc和argv来传递命令行参数。为了不同程序之间参数的规范性，推荐使用一个短横线开头，后面使用单字母或数字的形式。标准库中有个专门的函数来处理命令行参数\ ``getopt``

.. code:: c

   #include<unistd.h>

   int getopt(int argc, char *const argv[], const char *optstring);
   extern char *optarg;
   extern int optind, opterr, optopt;

optstring是一个字符列表，每个字符代表一个单字符选项。如果一个字符后面紧跟一个冒号，则表示下一个值是这个选项的参数。

.. code:: c

   #include<stdio.h>
   #include<unistd.h>
   #include<stdlib.h>

   int main(int argc, char *argv[])
   {
       int opt;

       while ((opt = getopt(argc, argv, ":if:lr")) != -1) {
           switch (opt) {
               case 'i':
               case 'l':
               case 'r':
                   printf("option: %c\n", opt);
                   break;
               case 'f':
                   printf("filename: %s\n", optarg);
                   break;
               case ':':
                   printf("option needs an value\n");
                   break;
               case '?':
                   printf("unknown option: %c\n", optopt);
                   break;
           }
       }
       for (; optind < argc; optind++)
           printf("argument: %s\n", argv[optind]);
       exit(0);
   }

用下面的输入来分析上面的例子。

-  getopt函数处理完所有选项后返回-1，没处理完则返回选项。
-  getopt实际上会把argv的顺序重排，把选项排在前面。非选项排在后面，这里\ ``'hi there'``\ 就被排在后面了
-  optind指向下一个待处理的参数的索引。
-  f后面有冒号，所以选项后面有关联值，且这个关联值存在optarg中。
-  遇到一个无法识别的选项则返回?，并且把这个选项存到optopt中。

::

   leo@ubuntu:~/c_test$ ./argopt -i -lr 'hi there' -f fred.c -q
   option: i
   option: l
   option: r
   filename: fred.c
   unknown option: q
   argument: hi there

如果选项需要关联值，而用户未提供则会有两种情况：

-  optstring以冒号开头，返回冒号
-  否则返回问号

显然我们这里返回冒号，表示没有提供关联值

::

   leo@ubuntu:~/c_test$ ./argopt -f
   option needs an value

getopt_long
~~~~~~~~~~~

有些程序使用的是双划线的长选线，标准库中的getopt_long可以处理这样的选项。

.. code:: c

   #include<stdio.h>
   #include<unistd.h>
   #include<stdlib.h>

   #define _GNU_SOURCE
   #include<getopt.h>

   int main(int argc, char *argv[])
   {
       int opt;
       struct option longopts[] = { 
           {"initialize", 0, NULL, 'i'},
           {"file", 1, NULL, 'f'},
           {"list", 0, NULL, 'l'},
           {"restart", 0, NULL, 'r'},
           {0,0,0,0}};

       while ((opt = getopt_long(argc, argv, ":if:lr", longopts, NULL)) != -1) {
           switch (opt) {
           case 'i':
           case 'l':
           case 'r':
               printf("option: %c\n", opt);
               break;
           case 'f':
               printf("filename: %s\n", optarg);
               break;
           case ':':
               printf("option needs a value\n");
               break;
           case '?':
               printf("unknown option: %c\n", optopt);
               break;
           }
       }   
       for (; optind < argc; optind++)
           printf("argument: %s\n", argv[optind]);
       exit(0);
   }

getopt_long比getopt多两个参数。第三个参数是一个结构数组，这个数组必须以一个全为0的结构结尾。第四个参数是一个指针变量，它可以作为optind的长选项版本使用。

第三个参数的结构定义在\ ``getopt.h``\ 中，且该头文件必须与常量\ ``_GNU_SOURCE``\ 一同包含进来。

.. code:: c

   struct option {
       const char *name;
       int has_tag;
       int *flag;
       int val;
   }

结构成员的含义：

-  name：长选项的名字
-  has_arg：是否带参数，0不带，1表示必须有一个参数，2表示有一个可选参数
-  flag：如果是NULL，getopt_long返回val的值。否则函数本身返回0，并将val值写入flag指向的变量。
-  val：选项对应的值。

在使用长选项的时候，可以不用输入完全的内容，只要能够识别就行。

::

   leo@ubuntu:~/c_test$ ./longopt --init -l 'hi there' --file fred.c -q
   option: i
   option: l
   filename: fred.c
   unknown option: q
   argument: hi there

4.2 环境变量
------------

.. code:: c

   #include<stdlib.h>

   char *getenv(const char *name);
   int putenv(const char *string);

环境变量由\ ``名字=值``\ 的形式存储。

-  getenv的参数是\ ``名字``\ ，返回\ ``值``\ 。注意返回的字符串是存在getenv提供的一个静态空间中的，如果要进一步使用，先拷贝一下，避免覆盖。如果没有，则返回null
-  putenv以\ ``名字=值``\ 形式的字符串设置环境变量。

.. code:: c

   #include<stdio.h>
   #include<string.h>

   int main(int argc, char *argv[])
   {
       char *var, *value;

       if (argc == 1 || argc > 3) {
           fprintf(stderr, "usage: environ var [value]\n");
           exit(1);
       }   

       var = argv[1];
       value = getenv(var);
       if (value)
           printf("Variable %s has value %s\n", var, value);
       else
           printf("Variable %s has no value\n", var);

       if (argc == 3) {
           char *string;
           value = argv[2];
           string = malloc(strlen(var) + strlen(value) + 2); 
           if (!string) {
               fprintf(stderr, "out of memory\n");
               exit(1);
           }
           strcpy(string, var);
           strcat(string, "=");
           strcat(string, value);
           printf("Calling putenv with %s\n", string);
           if (putenv(string) != 0) {
               fprintf(stderr, "putenv failed\n");
               free(string);
               exit(1);
           }
           value = getenv(var);
           if (value)
               printf("New value of %s is %s\n", var, value);
           else
               printf("New value of %s is null??\n", var);
       }   
       exit(0);
   }

注意环境仅对程序本身有效。变量的值不会从子进程（./environ程序）传播到父进程（shell）

::

   leo@ubuntu:~/c_test$ ./environ HOME
   Variable HOME has value /home/leo
   leo@ubuntu:~/c_test$ ./environ DS
   Variable DS has no value
   leo@ubuntu:~/c_test$ ./environ DS cd
   Variable DS has no value
   Calling putenv withL DS=cd
   New value of DS is cd
   leo@ubuntu:~/c_test$ ./environ DS
   Variable DS has no value

另外，我们可以用environ来访问环境变量。environ是一个以null结尾的字符串数组。

.. code:: c

   #include<stdlib.h>

   extern char **environ;

下面的程序可以输出当前环境的所有环境变量。

.. code:: c

   #include<stdlib.h>
   #include<stdio.h>

   extern char **environ;

   int main()
   {
       char **env = environ;
       
       while (*env) {
           printf("%s\n", *env);
           env++;
       }   
       exit(0);
   }

4.3 日期和时间
--------------

time
~~~~

.. code:: c

   #include<time.h>

   time_t time(time_t *tloc);

time函数返回当前秒数。如果tloc不为空，还会把值放到tloc指向的地方。下面的程序在20秒内，每个两个输出一下。

.. code:: c

   #include<time.h>
   #include<stdio.h>
   #include<unistd.h>
   #include<stdlib.h>

   int main()
   {
       int i;
       time_t the_time;

       for (i = 1; i <= 10; i++) {
           the_time = time((time_t *)0);
           printf("The time is %ld\n", the_time);
           sleep(2);
       }   
       exit(0);
   }

difftime
~~~~~~~~

.. code:: c

   #include<time.h>

   double difftime(time_t time1, time_t, tim2);

要计算时间差不要直接用time_t类型的变量进行计算，而是用difftime函数，返回time1-time2的值。

gmtime localtime mktime
~~~~~~~~~~~~~~~~~~~~~~~

.. code:: c

   #include<time.h>

   struct tm *gmtime(const time_t *timeval);
   struct tm *localtime(const time_t *timeval);
   time_t mktime(struct tm *timeptr);

-  ``gmtime``\ 返回UTC中央时区的tm结构
-  ``localtime``\ 和gmtime一样，不过时间是当地时区的
-  ``mktime``\ 和gmtime相反，把一个tm结构转换为秒数

tm结构至少包含如下成员（类型都是int）：

-  ``tm_sec`` 秒，0-61
-  ``tm_min`` 分，0-59
-  ``tm_hour`` 小时，0-23
-  ``tm_mon`` 月份，0-11
-  ``tm_year`` 从1900年开始算的年份
-  ``tm_wday`` 星期几，0-6
-  ``tm_mday`` 月份中的日期，1-31
-  ``tm_yday`` 年中的日期，0-365
-  ``tm_isdst`` 是否是夏令时

下面的程序使用gmtime获取详细时间信息并输出

.. code:: c

   #include<time.h>
   #include<stdio.h>
   #include<stdlib.h>

   int main()
   {
       struct tm *tm_ptr;
       time_t the_time;

       (void) time(&the_time);
       tm_ptr = gmtime(&the_time);

       printf("Raw time is %ld\n", the_time);
       printf("gmtime gives:\n");
       printf("date: %02d/%02d/%02d\n", tm_ptr->tm_year, tm_ptr->tm_mon+1, tm_ptr->tm_mday);
       printf("time: %02d:%02d:%02d\n", tm_ptr->tm_hour, tm_ptr->tm_min, tm_ptr->tm_sec);
       exit(0);
   }

运行程序，输出

::

   leo@ubuntu:~/c_test$ ./a.out 
   Raw time is 1572535479
   gmtime gives:
   date: 119/10/31
   time: 15:24:39

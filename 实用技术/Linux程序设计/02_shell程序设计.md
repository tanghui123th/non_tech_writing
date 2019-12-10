# 2 shell程序设计（未完）

## 2.1 重定向

三个文件描述符：

- 0：标准输入
- 1：标准输出
- 2：标准错误输出

下面的例子把标注输出和标准错误输出重定向到不同的文件（如果对一个不存在的pid用kill便会报错）

```
$ kill -HUP 1234 > killout.txt 2> killerr.txt
```

下面则是把标准输出和标准错误输出都重定向到同一个文件。注意操作符的顺序，这里的含义是：将标准输出重定向到killouterr.txt文件，然后将错误输出重定向到与标准输出相同的地方。

```
$ kill -1 1234 > killouterr.txt 2>&1
```

## 2.2 作为程序设计语言的shell

shell脚本程序本质上是被看做shell的标准输入，所以脚本中可以包含任何在交互式下能通过PATH使用到的linux命令。

下面来看第一个实例

```bash
# !/bin/sh

# first
# This file looks through all the files in the current
# directory for the string POSIX, and then prints the names of
# those files to the standard output.

for file in *
do
  if grep -q POSIX $file
  then
    echo $file
  fi
done

exit 0
```

## 2.3 shell的语法

### 变量

默认情况下，所有变量都被当做字符串存储。如果变脸的值中间包含空格，则需要用双引号括起来。

```bash
leo@ubuntu:~/c_test$ a="Yes Dear"
leo@ubuntu:~/c_test$ echo $a
Yes Dear
leo@ubuntu:~/c_test$ b=7+5
leo@ubuntu:~/c_test$ echo $b
7+5
```

注意$在单引号和双引号中的不同：单引号中不会替换，双引号中会替换。

```bash
# !/bin/bash

myvar="Hi there"

echo $myvar
echo "$myvar"   # 变量替换
echo '$myvar'   # 不会替换
echo \$myvar

echo Enter some text
read myvar
echo '$myvar' now equals to $myvar
exit 0
```

我们运行一下

```bash
leo@ubuntu:~/c_test/cp2$ ./test.sh 
Hi there
Hi there
$myvar
$myvar
Enter some text
Hello World
$myvar now equals to Hello World
```

下面是一些环境变量的含义：

- $PS1：命令提示符
- $PS2：二级提示符，用来提示后续的输入，一般是>
- $IFS：输入域分隔符。当shell读取输入时，用来分割单词的字符

下面是一些脚本中的变量：

- \$0, \$1, $2...：shell脚本的名字和第1，2个...参数
- $#：传递给脚本的参数的个数
- $$：脚本的进程号。可以用来生成一个唯一的临时文件
- $*：参数列表
- $@：用一个空格分割的参数列表

下面的例子说明了部分参数的使用

```bash
# !/bin/bash

salutation="Hello"
echo $salutation
echo "The program $0 is now running"
echo "The first parameter was $1"
echo "The second parameter was $2"
echo "The parameter list was $*"

exit 0
```

运行结果

```bash
leo@ubuntu:~/c_test/cp2$ ./t2 foo bar bam
Hello
The program ./t2 is now running
The first parameter was foo
The second parameter was bar
The parameter list was foo bar bam
```

如果你觉得本笔记对你有所帮助，欢迎转载分享给更多的人。请注明出处哦~
最后欢迎star [github仓库](https://github.com/LeoSirius/notes)

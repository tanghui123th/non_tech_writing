# 5 shell的类型、关系和内建命令

## 5.1 shell的类型

默认的交互shell会在用户登录时启动。还有另外一个默认shell是/bin/sh，它作为默认的系统shell，用于那些需要在系统启动时使用的系统shell脚本。CentOS将/bin/sh软连接到bash，ubuntu则软连接到dash。

所以构建shell脚本的时候最好在第一行注释写清楚需要使用的shell。

## 5.2 shell的父子关系

生成子shell时，只有部分父进程的环境被复制到了子shell环境中。

### 进程列表

在一行中输入多条命令依次执行，可以用命令列表实现

```bash
leo@192 ~ $ pwd ; ls ; cd /etc ; pwd ; cd  ; pwd ; ls
/Users/leo
Desktop       Downloads     Library       Music         Postman       Seafile       dev
Documents     Github        Movies        Pictures      Public        YouCompleteMe
/etc
/Users/leo
Desktop       Downloads     Library       Music         Postman       Seafile       dev
Documents     Github        Movies        Pictures      Public        YouCompleteMe
```

进程列表就是用括号包围起来的一组命令，它能够创建子shell来执行这些命令。`echo $BASH_SUBSHELL`输出0，表示没有子shell。输出更大的数字，则表示有子shell。

```bash
leo@192 ~ $ echo $BASH_SUBSHELL
0
leo@192 ~ $ ( pwd ; echo $BASH_SUBSHELL)
/Users/leo
1
leo@192 ~ $ ( pwd ; (echo $BASH_SUBSHELL ))
/Users/leo
2
```

### 后台模式和子shell混合使用

进程列表、协程和管道都利用了子shell

#### 后台模式

在命令末尾加&可以让命令在后台运行

```bash
leo@192 ~ $ sleep 10 &
[1] 75060
leo@192 ~ $ jobs
[1]+  Running                 sleep 10 &
leo@192 ~ $ jobs
[1]+  Done                    sleep 10
```

jobs有参数-l可以显示PID

```
leo@192 ~ $ jobs -l
[1]+ 75107 Running                 sleep 5 &
```

#### 将进程列表置入后台

后台模式有一个问题，就是后台程序如果有输出，会直接输出在shell上，可能影响当前的操作。一个解决办法是把进程列表放在后台模式。这样既可以在子shell中处理繁重的工作，又不会让子shell的IO影响父shell。

#### 协程coproc

协程同时做了两件事情：在后台生成一个子shell，在这个子shell中执行命令

```bash
root@a57436bf6370:~# coproc sleep 10
[1] 17155     # 作业号和进程ID
root@a57436bf6370:~# jobs
[1]+  Running                 coproc COPROC sleep 10 &
```

COPROC是默认的进程名字，如果要自定义进程名，可以用下面的语法

```bash
root@a57436bf6370:~# coproc My_Job { sleep 10; }
[1] 17163
root@a57436bf6370:~# jobs
[1]+  Running                 coproc My_Job { sleep 10; } &
```

## 5.3 理解shell的内建命令

### 外部命令

有时也称为文件系统命令。是存在于bash shell之外的命令。常位于/bin, /usr/bin, /sbin, /usr/sbin中。当执行外部命令时，会创建一个子进程。

### 内建命令

内建命令不需要子进程来执行，它们和shell编译在了一起，作为shell工具的组成部分存在。内建命令的执行速度更快，效率更高。

注意有些命令有多种实现，如echo和pwd既有内建命令，也有外部命令。type -a会显示命令的两种实现，而which只会显示外部命令文件。

```bash
root@a57436bf6370:~# type -a echo
echo is a shell builtin
echo is /bin/echo
root@a57436bf6370:~# which echo
/bin/echo
root@a57436bf6370:~# type -a pwd
pwd is a shell builtin
pwd is /bin/pwd
root@a57436bf6370:~# which pwd
/bin/pwd
root@a57436bf6370:~# type -a ps
ps is /bin/ps
root@a57436bf6370:~# which ps
/bin/ps
root@a57436bf6370:~# type -a cd
cd is a shell builtin
root@a57436bf6370:~# which cd
root@a57436bf6370:~#
```

### history

这是一条有用的内建命令。注意命令的历史记录是先存放在内存中的，当shell退出时才被写入到历史文件`.bash_history`中。如果要提前写入文件，可以用加-a选项。

要执行历史记录中的命令，可以用惊叹号加编号

```bash
root@a57436bf6370:~# history
    1  run.sh fetch
    2  run.sh compile
    3  run.sh python-env /data/dev/seahub/manage.py migrate
    4  run.sh
    5  run.sh python-env /data/dev/seahub/manage.py createsuperuser
    6  run.sh
    7  cd data/dev/seahub/frontend/
    8  npm run dev
    9  exit
   10  pip install pycrypto
   11  ls
   12  cd data
root@a57436bf6370:~# !11
ls
scripts
```

### alias

这是另一条内建命令，可以创建别名。选项-p可以查看已有的别名

```bash
root@a57436bf6370:~# alias -p
alias egrep='egrep --color=auto'
alias fgrep='fgrep --color=auto'
alias grep='grep --color=auto'
alias l='ls -CF'
alias la='ls -A'
alias ll='ls -alF'
alias ls='ls --color=auto'
```

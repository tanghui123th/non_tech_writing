# 6 使用Linux环境变量

## 6.1什么是环境变量

环境变量用来存储有关shell会话和工作环境的信息

### 全局环境变量和局部环境变量

全局环境变量对所有shell会话都是可见的，局部环境变量则只对创建它们的shell可见。系统环境变量基本上都是用大写，以区别于普通用户的环境变量。

显示环境变量的三个命令：

- set：显示全局变量、局部变量、用户自定义变量，且会进行字母排序
- env：只输出全局变量，且不会排序
- printenv：同上，且可以输出单个环境变量的值

另外echo输出变量的值也可以输出全局变量

```
root@a57436bf6370:/etc# printenv HOME
/root
root@a57436bf6370:/etc# echo $HOME
/root
```

## 6.2 设置用户定义变量

一旦启动shell（或执行一个脚本），则在这个shell中创建的变量为局部变量，可以用export命令输出为全局变量。

> 注意在子shell中修改全局环境变量不会影响父shell中该变量的值。子shell甚至不能用export改变父shell中全局环境变量的值。

## 6.3 删除环境变量

unset命令删除环境变量。注意用变量的值时加$，操作变量不加$。

> 注意如果在子shell中删除一个全局环境变量，则只在子shell中生效。父shell依旧可以使用该环境变量。

## 6.4 设置PATH环境变量

当在shell命令行界面中输入一个外部命令时，shell搜索在PATH中定义的目录找到对应的程序。如果命令的位置没有包括在PATH变量中，那么如果不使用绝对路径的话，shell是找不到的。

PATH添加新路径的方法：

```
PATH=$PATH:/home/leo/scripts
```

如果要在子shell中用新添加的目录，需要把PATH导出。

## 6.5 定位系统环境变量

上一小节中修改的PATH退出或重启后变会失效。在启动shell时，shell会去执行启动文件中的命令。启动bash shell有三种方式：

- 登录时的默认shell
- 非登录的交互shell
- 运行脚本的非交互shell

### 登录shell

登录shell会从5个不同的启动文件中读取命令

- /etc/profile：系统的主启动文件
- $HOME/.bash_profile
- $HOME/.bashrc
- $HOME/.bash_login
- $HOME/.profile

后4个文件时针对用户的。并不是所有发行版都有全部后四个文件。

### 非登录交互式shell

这类shell只会检查HOME命令下的.bashrc文件

### 非交互式shell

这类shell环境变量的两个来源：

- 父shell导出的全局变量
- BASH_ENV中设置的变量

### 环境变量持久化

再了解了上面三种shell及变量的读取方式后，我们便可以知道怎么进行变量的持久化了。

对于全局环境变量（所有用户都是用的全局环境变量）来说，/etc/profile文件会随着系统的升级而更新，所以不是放持久环境变量的地方。最好是在/etc/profile.d目录中创建一个以.sh结尾的文件，把所有新的或修改过的全局环境变量设置放在这个文件里。

对于单个用户的环境变量，可以放在$HOME/.bashrc中。但是如果设置了BASH_ENV变量，除非它指向的是$HOME/.bashrc，否则应该将非交互式shell的用户变量放在其他地方。

## 6.7 数组变量

我们直接来看用法

```
leo@192 ~ $ mytest=(one two three four five)
leo@192 ~ $ echo $mytest
one
leo@192 ~ $ echo ${mytest[2]}
three
leo@192 ~ $ echo ${mytest[*]}
one two three four five
leo@192 ~ $ unset mytest[2]
leo@192 ~ $ echo ${mytest[*]}
one two four five
leo@192 ~ $ echo ${mytest[2]}   # 删除一个元素后，后面的元素不会补上来，2这里是空着的

leo@192 ~ $ echo ${mytest[3]}
four
leo@192 ~ $ unset mytest
leo@192 ~ $ echo ${mytest[*]}
```
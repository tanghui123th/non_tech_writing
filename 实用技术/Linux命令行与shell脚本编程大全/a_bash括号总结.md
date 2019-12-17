# bash括号总结

## 总结

- 括号
  一般在命令替换的时候使用
- 双括号
  在比较过程中使用高级数学表达式
- 方括号
  用于逻辑分支中的条件呢判断
- 双方括号
  支持字符串比较、正则匹配

## 括号

```bash
#!/bin/bash

today=$(date +%y%m%d)
touch log.$today
```

## 双括号

```bash
#!/bin/bash

val1=10
if (( $val1 ** 2 > 90 )); then
    ((val2 = $val1 ** 2))
    echo "The square of $val1 is $val2"
fi
```

```bash
leo@ubuntu:~/sh_test$ ./test.sh
The square of 10 is 100
```

### 方括号

### 双方括号

```bash
#!/bin/bash
# using patter matching
#
if [[ $USER == l* ]]; then
    echo "Hello $USER"
else
    echo "Sorry, I do not know you"
fi
```

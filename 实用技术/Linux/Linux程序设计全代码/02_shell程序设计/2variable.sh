#!/bin/bash

myvar="Hi there"

echo $myvar
echo "$myvar"
echo '$myvar'
echo \$myvar

echo Enter some text
read myvar

echo '$myvar' now equals $myvar
exit 0

# 输出，可以看到单引号和斜杠后面的$原样输出
# Hi there
# Hi there
# $myvar
# $myvar
# Enter some text
# 123
# $myvar now equals 123

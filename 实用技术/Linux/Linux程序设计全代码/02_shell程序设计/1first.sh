#!/bin/bash

# 如果当前目录下只有这个文件有“POSIX”，则只会输出这个文件的名字

for file in *
do
    if grep -q POSIX $file
    then
        echo $file
    fi
done

exit 0

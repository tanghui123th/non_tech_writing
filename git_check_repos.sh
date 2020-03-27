#!/bin/bash

RED='\033[0;31m'
NC='\033[0m' # No Color

for dirent in `ls $1`
do
    dir=$(pwd)"/"$dirent
    if [ -d "$dir" ]; then
        cd $dir
        if [ -d ".git" ]; then
            echo -e "${RED}CHECK GIT REPO: $(pwd) ${NC}"
            git status
            echo -e "\n"
        fi
        cd ..
    fi
done

#!/bin/bash

CC=$1
CC_FLAGS=$2

srcs=($3)
objs=($4)

for ((i = 0; i < "${#srcs[@]}"; i++))
    do
        echo Building ${objs[$i]} from ${srcs[$i]}
        echo ${CC} ${CC_FLAGS} -c ${srcs[$i]} -o ${objs[$i]}
        ${CC} ${CC_FLAGS} -c ${srcs[$i]} -o ${objs[$i]}
        echo Built ${objs[$i]} $'\n'
    done
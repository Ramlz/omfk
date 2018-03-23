#!/bin/bash

CC=$1
CC_FLAGS=$2

srcs=($3)
objs=($4)

incs=($5)
target_incs=($6)

echo [COPYING] header files to build directory
for ((i = 0; i < "${#incs[@]}"; i++))
    do
        cp ${incs[$i]} ${target_incs[$i]}
    done

for ((i = 0; i < "${#srcs[@]}"; i++))
    do
        echo [BUILDING] \"${objs[$i]}\" from \"${srcs[$i]}\"
        echo ${CC} ${CC_FLAGS} -c ${srcs[$i]} -o ${objs[$i]}
        ${CC} ${CC_FLAGS} -c ${srcs[$i]} -o ${objs[$i]}
        if [ $? -eq 0 ]; then
            echo "[BUILT]    \"${objs[$i]}\""$'\n'
        else
            echo "[HALT]     compilation terminated!"
            exit 228
        fi
    done

exit 0

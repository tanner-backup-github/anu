#!/bin/bash

if [ ${#@} -lt 3 ]
then
    echo "Supply at least three arguments"
    exit
fi

for arg in "${@:3}"
do
    sed -i "s/$1/$2/g" $arg
done

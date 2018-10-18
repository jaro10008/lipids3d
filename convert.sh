#!/bin/bash

random_name=pov_`hexdump -n 8 -e '2/4 "%08X"' /dev/urandom`

mkdir -p /tmp/$random_name



for file in `ls $1`
do
	./simulator -g $1/$file /tmp/$random_name/$file.pov
	povray +I/tmp/$random_name/$file.pov +O$2/$file.png +W512 +H512 +A
done

rm -rf /tmp/$random_name


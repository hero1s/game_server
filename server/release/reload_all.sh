#!/bin/bash

root_dir=`pwd`

all_dir=("./lobby_server" "./center_server/")

tLen=${#all_dir[@]}
for ((i=0;i<${tLen};i++))
do
	dirname=${all_dir[$i]}
	cd $dirname
	./reload.sh;
	cd $root_dir;
done





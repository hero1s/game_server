#!/bin/bash

root_dir=`pwd`

all_dir=("./center_server" "./lobby_server")

tLen=${#all_dir[@]}
for ((i=0;i<${tLen};i++))
do
	dirname=${all_dir[$i]}
	cd $dirname
	./restart_game.sh;
	cd $root_dir;
done









#!/usr/bin/env bash

root_dir=`pwd`
rm ./release/center_server/centerServer -f

all_dir=("./servers/center_server/build"
	     )

tLen=${#all_dir[@]}
while getopts rR opt
do
	case $opt in
		r)
			for ((i=0;i<${tLen};i++))
			do
				dirname=${all_dir[$i]}
				cd $dirname
				./rebuild.sh -r;
				cd $root_dir;
			done
			
            exit 0
            ;;
		*)
			;;
	esac
done
for ((i=0;i<${tLen};i++))
do
	dirname=${all_dir[$i]}
	cd $dirname
	./rebuild.sh;
	cd $root_dir;
done





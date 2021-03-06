#!/usr/bin/env bash

root_dir=`pwd`
rm ./release/dbagent_server/dbagentServer -f

all_dir=("./servers/dbagent_server/build")

while getopts rR opt
do
	case $opt in
		r)
			for var in ${all_dir[*]}
			do
				dirname=${var}
				cd $dirname
				sh ./rebuild.sh -r;
				cd $root_dir;
			done
			
            exit 0
            ;;
		*)
			;;
	esac
done
for var in ${all_dir[*]}
do
	dirname=${var}
	cd $dirname
	sh ./rebuild.sh;
	cd $root_dir;
done


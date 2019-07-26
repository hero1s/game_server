#!/bin/bash

while getopts rR opt
do
	case $opt in
		r)
		    rm CMakeCache.txt -f
            rm CMakeFiles -rf
            cmake ./
			make clean
			;;
		R)
            rm CMakeCache.txt -f
            rm CMakeFiles -rf
            cmake ./
			make clean
			;;
		*)
			;;
	esac
done

make -j 8

cd $root_dir

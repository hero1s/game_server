#!/bin/sh
source ../config.sh

if [ $# -gt 0 ]; then
	echo "param num:"$#
	for arg in $*
	do
		echo $arg
		svrid=$arg
		./stop.sh $svrid
		sleep 2
		./checkrun_server.sh $svrid
	done
else
	echo "no param,restart all center server"
	./stop.sh
	sleep 2
	tLen=${#all_center_svrs[@]}
    echo "center svrs num:"${tLen}
    for ((i=0;i<${tLen};i++))
    do
        svrid=${all_center_svrs[$i]}
        ./checkrun_server.sh $svrid
    done
fi



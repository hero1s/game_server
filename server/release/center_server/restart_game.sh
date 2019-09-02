#!/bin/sh
source ../config.sh

ulimit -c unlimited

if [ $# -gt 0 ]; then
	echo "param num:"$#
	for arg in $*
	do
		echo $arg
		svrid=$arg
		sh ./stop.sh $svrid
		sleep 2
		sh ./checkrun_server.sh $svrid
	done
else
	echo "no param,restart all center server"
	sh ./stop.sh
	sleep 2
	tLen=${#all_center_svrs[@]}
    echo "center svrs num:"${tLen}
    for ((i=0;i<${tLen};i++))
    do
        svrid=${all_center_svrs[$i]}
        sh ./checkrun_server.sh $svrid
    done
fi



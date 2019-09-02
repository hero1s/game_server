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
	echo "no param,restart all dbagent server"
	sh ./stop.sh
	sleep 2
	tLen=${#all_dbagent_svrs[@]}
    echo "dbagent svrs num:"${tLen}
    for ((i=0;i<${tLen};i++))
    do
        svrid=${all_dbagent_svrs[$i]}
        sh ./checkrun_server.sh $svrid
    done
fi



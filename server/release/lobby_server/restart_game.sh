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
	echo "no param,restart all lobby server"
	./stop.sh
	sleep 2
	#重启全部大厅服
	tLen=${#all_lobby_svrs[@]}
    echo "lobby svrs num:"${tLen}
    for ((i=0;i<${tLen};i++))
    do
        svrid=${all_lobby_svrs[$i]}
        ./checkrun_server.sh $svrid
    done
fi
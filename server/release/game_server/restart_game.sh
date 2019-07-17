#!/bin/sh
source ../config.sh

if [ $# -gt 0 ]; then
	echo "param num:"$#
	for arg in $*
	do
		svrid=$arg
		./stop.sh $svrid
		sleep 2
		./checkrun_server.sh $svrid
	done	
else
	echo "no param,restart all game server"
	tLen=${#all_game_svrs[@]}
	#关闭全部游戏服
    ./stop.sh
	sleep 2
	#重启全部游戏服
	echo "svrs num:"${tLen}
    for ((i=0;i<${tLen};i++))
    do
        svrid=${all_game_svrs[$i]}
        ./checkrun_server.sh $svrid
    done
fi	




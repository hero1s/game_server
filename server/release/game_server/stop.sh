#!/bin/sh
source ../config.sh

if [ $# -gt 0 ]; then
	echo "param num:"$#
	for arg in $*
	do
		svrid=$arg
		echo "stop svr:"$svrid
		cat pid_${svrid}.txt | xargs kill -10
	done
else
	echo "no param,stop all game server"
	tLen=${#all_game_svrs[@]}
	#关闭全部游戏服
	echo "stop svrs num:"${tLen}
	for ((i=0;i<${tLen};i++))
	do
		svrid=${all_game_svrs[$i]}
		echo "stop svr:"$svrid
		cat pid_${svrid}.txt | xargs kill -10
	done
fi
#!/bin/sh
source ../config.sh

if [ $# -gt 0 ]; then
	echo "param num:"$#
	for arg in $*
	do
		echo $arg
		svrid=$arg
		echo "reload svr:"$svrid
		cat pid_${svrid}.txt | xargs kill -12
	done
else
	echo "no param,reload all game server"

	tLen=${#all_game_svrs[@]}
	#关闭全部游戏服
	echo "reload svrs num:"${tLen}
	for ((i=0;i<${tLen};i++))
	do
		svrid=${all_game_svrs[$i]}
		echo "reload svr:"$svrid
		cat pid_${svrid}.txt | xargs kill -12
	done
fi
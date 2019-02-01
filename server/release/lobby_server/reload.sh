#!/bin/sh
source ../config.sh

tLen=${#all_lobby_svrs[@]}
echo "lobby svrs num:"${tLen}
for ((i=0;i<${tLen};i++))
do
	svrid=${all_lobby_svrs[$i]}
	echo "reload svr:"$svrid
	cat pid_${svrid}.txt | xargs kill -12
done

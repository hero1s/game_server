#!/bin/sh
source ../config.sh

if [ $# -gt 0 ]; then
	echo "param num:"$#
	for arg in $*
	do
		svrid=$arg
		echo "stop lobby svr:"$svrid
		cat pid_${svrid}.txt | xargs kill -10
	done
else
    tLen=${#all_lobby_svrs[@]}
    echo "stop lobby svrs num:"${tLen}
    for ((i=0;i<${tLen};i++))
    do
        svrid=${all_lobby_svrs[$i]}
        echo "stop lobby svr:"$svrid
        cat pid_${svrid}.txt | xargs kill -10
    done
fi







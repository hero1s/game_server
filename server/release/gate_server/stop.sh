#!/bin/sh
source ../config.sh

if [ $# -gt 0 ]; then
	echo "param num:"$#
	for arg in $*
	do
		svrid=$arg
		echo "stop gate svr:"$svrid
		cat pid_${svrid}.txt | xargs kill -10
	done
else
    tLen=${#all_gate_svrs[@]}
    echo "stop gate svrs num:"${tLen}
    for ((i=0;i<${tLen};i++))
    do
        svrid=${all_gate_svrs[$i]}
        echo "stop center svr:"$svrid
        cat pid_${svrid}.txt | xargs kill -10
    done
fi







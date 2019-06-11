#!/bin/sh
source ../config.sh

if [ $# -gt 0 ]; then
    echo "param num:"$#
	for arg in $*
	do
		echo $arg
		svrid=$arg
		pid=`cat pid_${svrid}.txt`
        echo "check gate svr:"$svrid" pid "${pid}
        processnum=`ps ax | awk '{ print $1 }' | grep ${pid}$ | grep server | grep -v grep | wc -l`
        if [ $processnum -lt 1 ];then
            ./gateServer --sid ${svrid} --cfg "../server_config/server_config.lua" &
        else
            echo "restart gate server fail: "$svrid
        fi
	done
else
    tLen=${#all_gate_svrs[@]}
    echo "gate svrs num:"${tLen}
    for ((i=0;i<${tLen};i++))
    do
        svrid=${all_gate_svrs[$i]}
        pid=`cat pid_${svrid}.txt`
        echo "check gate svr:"$svrid" pid "${pid}
        processnum=`ps ax | awk '{ print $1 }' | grep ${pid}$  | grep -v grep | wc -l`
        if [ $processnum -lt 1 ];then
            ./gateServer --sid ${svrid} --cfg "../server_config/server_config.lua" &
        fi
    done
fi









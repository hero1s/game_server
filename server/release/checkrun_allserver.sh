#!/bin/bash
source ./config.sh
root_dir=`pwd`

while true
do
        tLen=${#all_dir[@]}
        for ((i=0;i<${tLen};i++))
        do
                dirname=${all_dir[$i]}
                enter_exec_shell_ $root_dir $dirname "checkrun_server.sh";
        done
sleep 30
done

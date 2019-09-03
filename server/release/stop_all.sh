#!/bin/bash
source ./config.sh
root_dir=`pwd`

tLen=${#all_dir[@]}
for ((i=0;i<${tLen};i++))
do
	dirname=${all_dir[$i]}
	enter_exec_shell_ $root_dir $dirname "stop.sh"
done






#!/bin/sh
source ../config.sh

tLen=${#all_dbagent_svrs[@]}
echo "dbagent svrs num:"${tLen}
for ((i=0;i<${tLen};i++))
do
	svrid=${all_dbagent_svrs[$i]}
	echo "reload svr:"$svrid
	cat pid_${svrid}.txt | xargs kill -12
done

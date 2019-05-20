#!/bin/bash
root_dir=`pwd`

all_dir=("./lobby_server" "./center_server/" "./dbagent_server")

#!/bin/bash
while true
do
        tLen=${#all_dir[@]}
        for ((i=0;i<${tLen};i++))
        do
                dirname=${all_dir[$i]}
                cd $dirname
                ./checkrun_server.sh;
                cd $root_dir;
        done
sleep 30
done

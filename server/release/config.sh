#!/usr/bin/env bash

#全部游戏服
all_game_svrs=( )

#全部大厅服
all_lobby_svrs=(1 )

#中心服
all_center_svrs=(8888 )

#db服
all_dbagent_svrs=(9999 )

all_dir=("./center_server" "./lobby_server" "./dbagent_server" "./game_server")

#进入目录执行脚本
function enter_exec_shell_(){
    root_dir=$1
    dirname=$2
    exesh=$3
	cd $dirname
	sh ./$exesh;
	cd $root_dir;
}
#进入目录组执行脚本
function enter_group_exec_shell_(){
    root_dir=$1
    dirnames=$2
    exesh=$3
    for var in ${dirnames[*]}
    do
        cd $var
        sh ./$exesh
        cd ../
    done
	cd $root_dir;
}





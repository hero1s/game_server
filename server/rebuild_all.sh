#!/usr/bin/env bash

root_dir=`pwd`

while getopts rR opt
do
	case $opt in
		r)
            ./rebuild_common.sh -r
            ./rebuild_game.sh -r
            ./rebuild_lobby.sh -r
            ./rebuild_center.sh -r
            ./rebuild_dbagent.sh -r
            exit 0
            ;;
		*)
			;;
	esac
done

./rebuild_common.sh
./rebuild_game.sh
./rebuild_lobby.sh
./rebuild_center.sh
./rebuild_dbagent.sh


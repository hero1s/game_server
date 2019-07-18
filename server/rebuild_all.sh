#!/usr/bin/env bash

root_dir=`pwd`

while getopts rR opt
do
	case $opt in
		r)
            sh ./rebuild_common.sh -r
            sh ./rebuild_lobby.sh -r
            sh ./rebuild_center.sh -r
            sh ./rebuild_dbagent.sh -r
            exit 0
            ;;
		*)
			;;
	esac
done

sh ./rebuild_common.sh
sh ./rebuild_lobby.sh
sh ./rebuild_center.sh
sh ./rebuild_dbagent.sh


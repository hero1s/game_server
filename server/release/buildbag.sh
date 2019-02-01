#!/bin/bash

echo  '打包'
rm -rf ./bag/*

cp game_server/gameServer               bag/
cp server_config/lua                    bag/ -R

while getopts lc opt
do
	case $opt in
		l)
            cp lobby_server/lobbyServer 	    	bag/
            ;;
        c)
            cp center_server/centerServer           bag/
            ;;
		*)
			;;
	esac
done

tar -zcvf bag_pkg/bag_$(date +%Y%m%d_%H%M).tar.gz bag

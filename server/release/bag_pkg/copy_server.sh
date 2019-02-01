#!/usr/bin/env bash
install ./bag/lobbyServer         ../lobby_server/
install ./bag/gameServer          ../game_server/
install ./bag/centerServer        ../center_server/
cp      ./bag/lua                 ../server_config/ -R



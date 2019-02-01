#!/usr/bin/env bash

fn=$1

echo "tar pkg file:"$fn
tar -vxf $fn

./copy_server.sh

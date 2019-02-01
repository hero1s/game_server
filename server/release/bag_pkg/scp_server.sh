#!/usr/bin/env bash

fn=$1

echo "scp pkg file:"$fn
scp ./$fn root@172.18.231.141:/data/server/

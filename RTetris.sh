#!/bin/bash

GAME_PATH=$1

if [ -z "$1" ]; then
    GAME_PATH=$(pwd)
    echo No path given using pwd path $GAME_PATH
fi

dosbox  $GAME_PATH/bin/RTetris.exe -conf $GAME_PATH/dosbox/dosbox.conf -exit

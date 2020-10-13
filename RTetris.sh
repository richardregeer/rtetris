#!/bin/bash

GAME_PATH=$1

if [ -z "$1" ]; then
    GAME_PATH=$(dirname "$0")
    echo No path given using path $GAME_PATH
fi

dosbox  $GAME_PATH/bin/RTetris.exe -conf $GAME_PATH/dosbox/dosbox.conf -exit

#!/bin/bash

if [ -z "$1" ]
then
    echo "Usage: ./compilelox.sh <script>"
else
    NAME="${1%.*}"
    ./build/parser $1 | llc -filetype=obj > $NAME.o
    gcc $NAME.o -o $NAME
fi
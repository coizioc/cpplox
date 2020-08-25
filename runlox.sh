#!/bin/bash

if [ -z "$1" ]
then
    echo "Usage: ./runlox.sh <script>"
else
    ./build/parser $1 | lli
fi
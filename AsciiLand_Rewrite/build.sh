#!/bin/bash

code="$PWD"
opts=-g
cd build > /dev/null
g++ $opts $code/main.c -o AsciiLand_Rewrite
cd $code > /dev/null

#!/bin/sh
### this is our single command test ###
../bin/rshell < mkdir tree
../bin/rshell < ls
../bin/rshell < echo good morning
../bin/rshell < echo test && echo record || exit
../bin/rshell < touch newfile.cpp
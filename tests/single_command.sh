#!/bin/sh
### this is our single command test ###
../bin/rshell < mkdir tree
../bin/rshell < ls
../bin/rshell < echo good morning
../bin/rshell < #good afternoon
../bin/rshell < echo hello #display comment
../bin/rshell < '\n'
../bin/rshell < touch newfile.cpp
../bin/rshell < exit

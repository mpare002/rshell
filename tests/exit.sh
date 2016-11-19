#!/bin/sh
### this is our exit command test ###
../bin/rshell < echo hello; exit
../bin/rshell < ls -a || exit || echo hello world
../bin/rshell < ls -a || exit && echo hello world
../bin/rshell < #exit
../bin/rshell < echo test && echo record || exit
../bin/rshell < echo world || exit #exit

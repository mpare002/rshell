#!/bin/sh
### this is our commented command test ###
../bin/rshell < echo hello #world
../bin/rshell < ls -a || echo hello world #testing comments on multiple commands
../bin/rshell < ps #exit
../bin/rshell < echo test # || ls
../bin/rshell < #
../bin/rshell < mkdir test_directory #create a test directory
../bin/rshell < rm -r test_directory #remove the directory
../bin/rshell < echo test && echo test2 # echo both statements
../bin/rshell < touch file.cpp #testing touch with a file name
../bin/rshell < #rm file.cpp #remove the file
../bin/rshell < #at file.cpp #testing cat on empty file
../bin/rshell < echo mkdir test_dir # echo test
../bin/rshell < ls -l #
../bin/rshell < exit

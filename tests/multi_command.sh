#!/bin/sh

#here is our multiple command test#

../bin/rshell < multi_command_test
../bin/rshell < echo hello world || ls -a; ps
../bin/rshell < ls -a && mkdir test_directory
../bin/rshell < ls #rm -r test_directory;ls 
../bin/rshell < echo hello #mkdir test_directory2 && echo made test directory
../bin/rshell < #rm -r test_directory2; ls
../bin/rshell < #echo testing clear; clear
../bin/rshell < echo || ls -m && echo testing || mkdir test_dir
../bin/rshell < ps && ls -r
../bin/rshell < cat nonexistantfile || echo testing invalid cat
../bin/rshell < ls -q; ps
../bin/rshell < touch test.cpp; echo created test.cpp && rm test.cpp
../bin/rshell < echo || mkdir || ls
../bin/rshell < ls -R && ps
../bin/rshell < ps; ps || ls && ps
../bin/rshell < ls -a || ls && ls
../bin/rshell < exit

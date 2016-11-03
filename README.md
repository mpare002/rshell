# rshell

## Collaborating Students
Michael Pare  
Ryan Ulep

## Coding Requirements
This program acts as a command shell and performs the following:

1. Prints a command prompt.
2. The programs run until exit is read in on the command line.
3. Reads in a command on one line. These commands take the form:

```
cmd = executable [argumentList] [connector cmd]
connector = || or && or ;
```

Note this program can take more than one command on a line and can be separated by connectors. There are no limits to the nuber of commands which can be chained together, and any combination of operators can be handled. 

## Known Bugs
1. This program currently does not allow the `cd` command. This will be handled in a future version. When the `cd` command is used in a command line, the program will not run.
2. 
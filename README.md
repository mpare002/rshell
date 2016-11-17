# rshell

## Collaborating Students
Michael Pare  
Tommy Shiels  
Ryan Ulep

## Coding Specifications
This program acts as a command shell and performs the following:

1. Prints a command prompt.
2. The programs run until exit is read in on the command line.
3. The rshell can also handle precendence and test operators.
4. Reads in a command on one line. These commands take the form:

```
cmd = executable [argumentList] [connector cmd]
connector = || or && or ;
```

Note this program can take more than one command on a line and can be separated by connectors.
There are no limits to the number of commands which can be chained together, and any combination of operators can be handled.   
5. The rshell can handle precedence operators, `(echo A && echo B) || (echo C && echo D)`.  
6. The rshell can execute test operations in either of the following formats: `test -e Makefile` and `[ -e Makefile ]`.

## Known Bugs
1. The rshell currently does not allow the `cd` command. This will be handled in a future version.
2. When the `cd` command is used in a command line, the rshell will not run.

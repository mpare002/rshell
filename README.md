# rshell

## Collaborating Students
Michael Pare  
Tommy Shiels  
Ryan Ulep

## Coding Specifications
This program acts as a command shell and performs the following:

1. Prints a command prompt.
2. The programs run until exit is read in on the command line.
3. The rshell can handle precedence operators, `(echo A && echo B) || (echo C && echo D)`.  
4. The rshell can execute test operations in either of the following formats: `test -e Makefile` and `[ -e Makefile ]`.
5. Reads in a command on one line. These commands take the form:
```
cmd = executable [argumentList] [connector cmd]
connector = || or && or ;
```

Note this program can take more than one command on a line and can be separated by connectors.
There are no limits to the number of commands which can be chained together, and any combination of operators can be handled.   

## Known Bugs
1. The following command structure causes an error: `[some test] connector (a precedence)`.
2. when entering a invalid directory as an argument several times for cd it will interfere with the PWD and OLDPWD variables 
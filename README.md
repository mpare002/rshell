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

## Known Bugs
1. The rshell currently does not allow the `cd` command. This will be handled in a future version.
2. When the `cd` command is used in a command line, the rshell will not run.
3. The rshell is unable to handle multiple commands being typed in a single command line with improper arguments, i.e. `&&||7789;;||`.
4. The rshell does not work when connectors do not have arguments on the left and right, i.e. `&& mkdir stuff` and `echo world ||`.
5. The rshell cannot handle leading or trailing connectors, i.e. `&& ls` or `echo world ||`.
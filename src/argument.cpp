#include "argument.h"
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <queue>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "parse.h"
#include <cstddef>
#include <algorithm>

using namespace std;
using namespace boost;

bool Executable::execute()
{
	// Declares varaibles needed
	unsigned i = 0;
	char *newArgs[10000];
	char *newInput;
	char *cmd;
	
	// Tokenizes individual commands
	argument.erase(argument.find_last_not_of(' ') + 1);
	
	while(argument.at(i) == ' ')
	{
		i++;
	}
	
	if(argument.substr(i, 4) == "exit")
	{
		if(i + 4 < argument.size())
		{
			i += 4;
		}
		else
		{
			exit(0);
		}
		while(argument.at(i) == ' ')
		{
			if(i + 1 >= argument.size())
			{
				exit(0);
			}
			else
			{
				i++;
			}
		}
		if(argument.at(i) == ';' || argument.at(i) == '&' || argument.at(i + 1) == '&' || argument.at(i) == '|' || argument.at(i + 1) == '|')
		{
			exit(0);
		}
	}
	
	i = 0;
	newInput = new char[argument.size() + 1];
	strcpy(newInput, argument.c_str());
	cmd = strtok(newInput, " \t");
	while(cmd != NULL)
	{
		newArgs[i] = cmd;
		cmd = strtok(NULL, " \t");
		i++;
	}
	newArgs[i] = NULL;	
	
	string tester(newArgs[0]);
	if(tester == "test")
	{
		//convert elements into strings for easier comparisons
		if(newArgs[1] == NULL)
		{
			//checks if there is no arguments after test 
			cout << "(False)" << endl;
			return false;
		}
		string flag(newArgs[1]);
		if(newArgs[1] == '\0')
		{
			//calling test -flag with no args
			cout << "(False)" << endl;
			return false;
		}
		else
		{
			delete[] newInput;
    		delete cmd;
			struct stat buff; //create buffer object for stat()
         if(flag == "-d")
			{
				stat(newArgs[2], &buff);
				if(S_ISDIR(buff.st_mode))
				{
					cout << "(True)" << endl;
					return true;
				}
				else
				{
					cout << "(False)" << endl;
					return false;
				}

			}
			if(flag == "-f")
			{
				stat(newArgs[2], &buff);
				if(S_ISREG(buff.st_mode))
				{
					cout << "(True)" << endl;
					return true;
				}
				else
				{
					cout << "(False)" << endl;
					return false;
				}

			}
			else
			{
				if(flag == "-e")
				{
					//cout << "e" << endl;
					if(stat(newArgs[2], &buff) == -1)
					{
						cout << "(False)" << endl;
						return false;
					}
					else
					{
						cout << "(True)" << endl;
						return true;
					}

				}
				else
				{
					//cout << "no flag" << endl;
					if(stat(newArgs[1], &buff) == -1)
					{
						cout << "(False)" << endl;
						return false;
					}
					else
					{	
						cout << "(True)" << endl;
						return true;
					}	
				}
			}
		}
	}
	//here we see if the argument is cd ----------------------------------------------------------------------------------
	else if(tester == "cd")
	{
		
		//first check if second argument is null so we know to return to home directory
		if(newArgs[1] == NULL)
		{
			setenv("OLDPWD", getenv("PWD"), 1); //set old pwd to current 
			char buff[100];
			chdir(getenv("HOME"));
			getcwd(buff, 100);
			setenv("PWD", buff, 1); //change PWD to new dir
			return true;
		}
		//next check to see if second argument is a '-' so we know to return to the previous dir
		string temp(newArgs[1]);
		if(temp == "-")
		{
			char buff[100];
			getcwd(buff, 100);
			setenv("PWD", getenv("OLDPWD"), 1);
			setenv("OLDPWD", buff, 1);
			chdir(getenv("PWD"));
			return true;
		}
		
		else //here we change our working directory to the new one
		{
			setenv("OLDPWD", getenv("PWD"), 1); //set old pwd to current 
			char buff[100];
			string current(getenv("PWD"));
			string tmp = current + "/" + temp;
			const char* newdir = tmp.c_str();
			//cout << tmp << endl;
			if(chdir(newdir) != 0)
			{
				perror("-rshell: cd");
				return false;
			}
			getcwd(buff, 100);
			setenv("PWD", buff, 1);

			return true;
		}
	

	}
	
	//Here is where we implement the system calls ------------------------------------------------------------------------
    bool result = true;
    int pid_child;
    int status;
   
    pid_child = fork();
    if(pid_child == -1)
    {
    	//an error in the fork process
    	perror("Fork");
    	
    	
    }
    if(pid_child == 0)
    {
    	//process is the child
      int ret = execvp(newArgs[0], newArgs);
    	
		if(ret == -1)
		{
			perror("invalid command");
		}
    	//if execvp returns that must have failed
    	
    	exit(-1);
    }
    
    if(pid_child > 0)
    {
    	//process is the parent
    	if (waitpid(pid_child, &status, 0) < 0)
    	{
 			perror("wait");
 			cout << "wait";
    	}
    }

    
	delete[] newInput;
    delete cmd;
    return result;
}


bool And::execute() 
{
	if(leftArg->execute() == true)
	{
		delete leftArg;
		bool result = rightArg->execute();
		delete rightArg;
		return result;
	}
	else
	{
		delete leftArg;
		delete rightArg;
      return false;
	}
}

bool Or::execute() 
{
	if(leftArg->execute() == false)
	{
		delete leftArg;
		bool result = rightArg->execute();
		delete rightArg;
		return result;
	}
	else
	{
		delete leftArg;
		delete rightArg;
		return true;
	}

}

bool Semicolon::execute() 
{
	bool result = leftArg->execute();
	delete leftArg;

	if (rightArg != NULL)
	{
		bool result2 = rightArg->execute();
		delete rightArg;
		
		return result2;
	}
	else
	{
		delete rightArg;
		return result;
	}
}

Test::Test(string t) {
	if (t.find('[') != string::npos)
	{
		string r = trim_copy(t);
		string u = r.substr(r.find('[') + 1);
		size_t s = u.find(']');
		
		if (s != string::npos) {
		  u.erase(s);
		}
		
		string v = trim_copy(u);
	   v = "test " + v;
		
      Arg = new Executable(v);
	}
}

bool Test::execute() {
	bool a = Arg->execute();
   delete Arg;
   return a;
}

Precedence::Precedence(string s) {
	Base* b = parse(s);
	Arg = b;
}

bool Precedence::execute() {
   bool a = Arg->execute();
   delete Arg;
   return a;
}

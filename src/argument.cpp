#include "argument.h"

using namespace std;

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
				//cout << "d" << endl;
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
				//cout << "f" << endl;
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
	
	//Here is where we implement the system calls
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

	if(rightArg != NULL)
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
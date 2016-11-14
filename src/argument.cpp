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
		if(newArgs[1] == NULL)
		{
			perror("not enough arguments");
		}
		else
		{
			delete[] newInput;
    		delete cmd;
			return test(newArgs[1], newArgs[2]);
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

bool Executable::test(char arr1[], char arr2[])
{
	struct stat buff;
	const char* arr1v = arr1;
	const char* arr2v= arr2;
	string temp1(arr1);
	string temp2(arr2);

	cout << temp1 << endl;
	cout << temp2 << endl;
	
	cout << "inside test" << endl;
	//this will check if file is a directory
	if(temp1 == "-d")
	{
		cout << "d" << endl;
		stat(*arr2v, &buff);
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
	//this will check if the file is a regular file
	else if(temp1 == "-f")
	{
		cout << "f" << endl;
		stat(*arr2v, &buff);
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
	//this will check if the file is a regualr file or directory regardless of -e flag
	else
	{
		if(temp1 == "-e")
		{
			cout << "e" << endl;
			if(stat(*arr2v, &buff) == -1)
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
			cout << "no flag" << endl;
			if(stat(*arr1v, &buff) == -1)
			{
				cout << "(False)!" << endl;
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
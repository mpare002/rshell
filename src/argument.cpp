#include "argument.h"

using namespace std;

bool Executable::execute()
{
	//conver argument string to char array
	/*int white_ = 0;
	for(unsigned int i = 0; i < argument.size(); ++i)
	{
		if (argument.at(i) == ' ')
		{
			++white_;
		}
	}
	char** newArr = new char*[white_ + 1];

	stringstream ss(argument);
	string buff;
	cout << "HELP" << endl;
	int i = 0;
	while(ss >> buff)
	{      
		char* temp = (char*)buff.c_str();
		newArr[i] = temp;
		++i;
	}

	newArr[i +1] = NULL;
	*/
	int i = 0;
	char *argv[10000];
	char *inputC;
	char *cmd;
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
		if(argument.at(i) == ';' || argument.at(i) == argument.at(i + 1) == '&' || argument.at(i) == argument.at(i + 1) == '|' )
		{
			exit(0);
		}
	}
	i = 0;
	inputC = new char[argument.size()];
	strcpy(inputC, argument.c_str());
	cmd = strtok(inputC, " \t");
	while(cmd != NULL)
	{
		argv[i] = cmd;
		cmd = strtok(NULL, " \t");
		i++;
	}
	argv[i] = NULL;



	//Here is where we implement the system calls
    bool result = true;
    int pid_child;
    //int status_child unused
    int status;

    pid_child = fork();
    //cout << pid_child << endl;
    if(pid_child == -1)
    {
    	cout << "error -1" << endl;
    	//an error in the fork process
    	perror("Fork");
    	
    	
    }
    if(pid_child == 0)
    {
    	cout << "execvp works..";
    	//process is the the child
    	int ret = execvp(argv[0], argv);
    	
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

    if(pid_child > 0)
    {
    	delete inputC;
    }
    return result;
}


bool And::execute() 
{
	if(leftArg->execute() == true)
	{
		return rightArg->execute();
	}
	else
	{
         return false;
	}
}

bool Or::execute() 
{
	if(leftArg->execute() == false)
	{
		return this->rightArg->execute();
	}
	else
	{
		return true;
	}

}

bool Semicolon::execute() 
{
	leftArg->execute();

	if(rightArg != NULL)
	{
		if(rightArg->execute() == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}
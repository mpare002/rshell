#include "argument.h"

using namespace std;

bool Executable::execute()
{
	//conver argument string to char array
	int white_ = 0;
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

	int i = 0;
	while(ss >> buff)
	{      
		char* temp = (char*)buff.c_str();
		newArr[i] = temp;
		++i;
	}
	newArr[i + 1] = '\0';


	//Here is where we implement the system calls
    bool result = true;
    pid_t pid_child;
    //int status_child unused
    int status;

    pid_child = fork();

    if(pid_child == -1)
    {
    	//an error in the fork process
    	perror("Fork");
    	exit(-1);
    }
    else if(pid_child == 0)
    {
    	//process is the the child
    	int ret = execvp(newArr[0], newArr);
		ret++;
    	//if execvp returns that must have failed
    	perror("invalid command");
    	exit(-1);
    }
    
    else
    {
    	//process is the parent
    	if (waitpid(pid_child, &status, 0) < 0)
    	{
 			perror("wait");
    	}
    }

    if (WEXITSTATUS(status) > 0)
    {
    	if(WEXITSTATUS(status) < 0)
    	{
    		result = false;
    	}
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

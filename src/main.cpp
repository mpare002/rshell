#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <queue>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "argument.h"
#include "parse.h"
#include <cstddef>
#include <stdexcept>

using namespace std;
using namespace boost;

typedef boost::tokenizer<boost::char_separator<char> > mytok;
typedef mytok::iterator tok_it;

int main()
{
	while(1) 
	{
		try 
		{
			// Declare variables
			char *login = getlogin();
			char host[200];
		    if(gethostname(host, 200) == -1) perror("hostname");
			string bash_input;
			
			// Output user information
			cout << login << "@" << host << ':' << getenv("PWD") << " $ ";
			getline(cin, bash_input);
	
			// Look for comments and remove comments if necessary
			size_t s = bash_input.find("#");
			if (s != string::npos) 
			{
				bash_input.erase(s);
			}
			
			// Trim left and right whitespace
			string bash_command = trim_copy(bash_input);
			
         
         // Now look for exit as only command and quit if necessary
			if (bash_command == "exit") 
			{
				exit(0);
			}
			
			// Create commands and execute bash line
			Base* start = parse(bash_command);
        	if (start != 0) 
        	{
				start->execute();
			}

			delete start;

		}
		catch (runtime_error &e) {
			cout << "-rshell: " << e.what() << endl;
		}
	}
	
	return 0;
}

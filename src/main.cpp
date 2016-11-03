#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#include <stack>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include "argument.h"
#include <cstddef>

using namespace std;
using namespace boost;

typedef boost::tokenizer<boost::char_separator<char> > mytok;
typedef mytok::iterator tok_it;

void connectors(string command, stack<string> &conn);
Base* parse(string input);
Base* constructOrder(stack<string> &con, stack<string> &commands);
Base* constructOrder(stack<string> &con, stack<string> &commands, Base* b);

int main()
{
	while(1) {
		// Declare variables
		char *login = getlogin();
		char host[200];
	    if(gethostname(host, 200) == -1) perror("hostname");
		string bash_input;
		
		// Output user information
		cout << login << "@" << host << "$ ";
		getline(cin, bash_input);
		
		// Look for comments and remove comments if necessary
		size_t s = bash_input.find("#");
		if (s != string::npos) {
			bash_input.erase(s);
		}
		
		// Trim left and right whitespace
		string bash_command = trim_copy(bash_input);
		
		// Now look for exit as only command and quit if necessary
		if (bash_command == "exit") {
			cout << "Leaving rshell" << endl;
			exit(0);
		}
		
		// Create commands and execute bash line
		Base* start = parse(bash_command);
	
		start->execute();
	}
	
	return 0;
}

Base* parse(string input) {
	
	// Declare container to hold commands and connectors
	stack<string> arguments;
	stack<string> connector;

	// Initiate tokenizer
    boost::char_separator<char> delim(";&&||");
    boost::tokenizer< boost::char_separator<char> > mytok(input, delim);
	
	// Build command container
    for (tok_it i = mytok.begin(); i != mytok.end(); ++i) {
        string temp = *i;
        string token = trim_copy(temp);
        arguments.push(token);
    }
    
    // Build connector container
    connectors(input, connector);
    
    // Create operation ordering system
    return constructOrder(connector,arguments);
}

Base* constructOrder(stack<string> &con, stack<string> &commands) {
	if (con.empty() && commands.size() == 1) {
		return new Executable(commands.top());
	}
	
	else {
		while (!con.empty()) {
			string connector = con.top();
			con.pop();
			
			string comm2 = commands.top(); // will get right since stack
			commands.pop();
			string comm1 = commands.top(); // will get left
			commands.pop();
			
			if (connector == ";") {
				Base* semicom = new Semicolon(new Executable(comm1), new Executable(comm2));
				return constructOrder(con, commands, semicom);
			}
			
			else if (connector == "&&") {
				Base* ancom = new And(new Executable(comm1), new Executable(comm2));
				return constructOrder(con, commands, ancom);
			}
			
			else if (connector == "||") { //Can be added back in later for furture assignments
				Base* orcom = new Or(new Executable(comm1), new Executable(comm2));
				return constructOrder(con, commands, orcom);
			}
		}
		
		return new Executable(commands.top());
	}
}

Base* constructOrder(stack<string> &con, stack<string> &commands, Base* b) {
	// This is the recursive construction, continues building tree, and returns highest pointer
	if (con.empty() && commands.empty()) {
		return b;
	}
	
	else {
		while (!con.empty()) {
			string connector = con.top();
			con.pop();
			
			string comm = commands.top(); // will get left since right was already made executable
			commands.pop();
			
			if (connector == ";") {
				Base* semicom = new Semicolon(new Executable(comm), b);
				return constructOrder(con, commands, semicom);
			}
			
			else if (connector == "&&") {
				Base* ancom = new And(new Executable(comm), b);
				return constructOrder(con, commands, ancom);
			}
			
			else if (connector == "||") { //Can be added back in for future assignments
				Base* orcom = new Or(new Executable(comm), b);
				return constructOrder(con, commands, orcom);
			}
		}
		
		return b;
	}
}

void connectors(string command, stack<string> &conn) {
  for (size_t i = 0; i < command.size(); i++) {
		if (command[i] == '|' && command[i+1] == '|') {
			conn.push("||");
		}
		else if (command[i] == ';') {
			conn.push(";");
		}
		else if (command[i] == '&' && command[i+1] == '&') {
			conn.push("&&");
		}
	}
}
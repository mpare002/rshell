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
#include "argument.h"
#include "parse.h"
#include <stdexcept>
#include <cstddef>

using namespace std;
using namespace boost;

typedef boost::tokenizer<boost::char_separator<char> > mytok;
typedef mytok::iterator tok_it;

Base* parse(string input) {
	
	// Declare container to hold commands and connectors
	queue<string> arguments;
	queue<string> connector;
	
	// Store "black boxing" commands here
	queue<string> blackbox;
	
	// Do pre-parsing for precedence here
	size_t j = input.find('(');
	while (j != string::npos) {
	   // Initiate tracking variable
		int t = -1;
      size_t a;
		
      // Start looping through remainder of string
		for (a = j; a < input.size(); ++a) {
			// Increases tracker to keep precedence in order
			if (input.at(a) == '(') {
				++t;
			}
			// Will check for corresponding parentheses
			if (input.at(a) == ')') {
               if (t != 0) {
					--t;
				}
				else {
					// Pull out substring and push into queue
					blackbox.push(input.substr(j+1, ((a - 1) - j)));
					// Replace removed portion with keyword "PREC"
               input.replace(j, ((a + 1) - j), "PREC");
               j = input.find('(');
            }
			}	
         if (a == input.size() - 1) {
            throw runtime_error("'(' missing ')'");
         }
      }
		j = input.find('(');
      cout << j << endl;
      cout << "input after j: " << input << endl;
	}
	
   // Do pre-parsing for test command here
	size_t k = input.find('[');
	
	while (k != string::npos) {
		// Initiate tracking variable
		int t = -1;
		
		// Start looping through remainder of string
		for (size_t a = k; a < input.size(); ++a) {
			
			// Increases tracker to keep precedence in order
			if (input.at(a) == '[') {
				++t;
			}
			
			// Will check for corresponding parentheses
			if (input.at(a) == ']') {
				if (t != 0) {
					--t;
				}
				else {
					// Pull out substring and push into queue
					blackbox.push(input.substr(k, ((a+1) - k)));
					// Replace removed portion with keyword "PREC"
					input.replace(k, ((a+1) - k), "TEST");
				}
			}
         if (a == input.size() - 1) {
            throw runtime_error("'[' missing ']'");
         }
		}
		k = input.find('[');
	}
cout << "input: " << input << endl;	
   // Initiate tokenizer
    boost::char_separator<char> delim(";&&||");
    boost::tokenizer< boost::char_separator<char> > mytok(input, delim);
	
	// Build command container
   vector<string> argtest;
    for (tok_it i = mytok.begin(); i != mytok.end(); ++i) {
        string temp = *i;
        string token = trim_copy(temp);
        arguments.push(token);
        argtest.push_back(token);
    }
    
    // Build connector container
    connectors(input, connector);
    
    if (arguments.empty()) {
    	return 0;
    }

    for (unsigned i = 0; i < argtest.size(); ++i) {
       if (argtest.at(i) == "") {
          throw runtime_error("syntax error: check arguments and connectors");
       }
    }
    // Create operation ordering system
    return constructOrder(connector, arguments, blackbox);
}

Base* constructOrder(queue<string> &con, queue<string> &commands, queue<string> &prectest) {
	cout << "enter construct 1" << endl;
   if (con.empty() && commands.size() == 1 && prectest.size() == 0) {
		cout << "enter a1" << endl;
      return new Executable(commands.front());
	}
cout << "passed if 1" << endl;	
	if (con.empty() && commands.size() == 1 && prectest.size() == 1) {
		if (commands.front() == "PREC") {
			cout << "enter a2" << endl;
         return new Precedence(prectest.front());
		}
		else {
			cout << "enter a3" << endl;
         return new Test(prectest.front());
		}
	}
	else {
		cout << "entered else" << endl;
      if (!con.empty()) {
			cout << "enter a4" << endl;
         for(unsigned int i = 0; i < con.size(); i++){
            cout << "con at " << i << " = " << con.front()<< endl;
         }
         string connector = con.front();
			con.pop();

         
			
			string comm1 = commands.front(); // will get left since queue
			commands.pop();
			string comm2 = commands.front(); // will get right
			commands.pop();
         
         cout << "a4 comm1: " << comm1 << endl;
         cout << "a4 comm2: " << comm2 << endl;
         // Create appropriate object types based on connectors and if no precedence or test
			if (prectest.empty()) {
            cout << "Enter prectest empty and connector: " << connector <<  endl;
            if (connector == ";") {
					Base* semicom = new Semicolon(new Executable(comm1), new Executable(comm2));
					return constructOrder(con, commands, prectest, semicom);
				}
				
				else if (connector == "&&") {
					cout << "entered and" << endl;
               Base* ancom = new And(new Executable(comm1), new Executable(comm2));
					return constructOrder(con, commands, prectest, ancom);
				}
				
				else if (connector == "||") {
					Base* orcom = new Or(new Executable(comm1), new Executable(comm2));
					return constructOrder(con, commands, prectest, orcom);
				}
			}
			// Create appropriate objects if a precedence or test is present
			else {
				cout << "Enter prectest not empty" << endl;
            // Initialize variables
				string str1 = "";
				string str2 = "";
				
				Base* b1 = 0;
				Base* b2 = 0;
				
            // Create appropriate objects
				if (comm1 == "PREC" || comm1 == "TEST") {
                cout << "enter str1 prectest" << endl;
                string str1 = prectest.front();
                prectest.pop();
					cout << "str1: " << str1 << endl;
					if (comm1 == "PREC") {
						b1 = new Precedence(str1);
					}
					if (comm1 == "TEST") {
						b1 = new Test(str1);
					}
				}
				
				if (comm2 == "PREC" || comm2 == "TEST") {
					cout << "enter str2 prectest" << endl;
               string str2 = prectest.front();
					prectest.pop();
					cout << "str2: " << str2 << endl;
					if (comm2 == "PREC") {
						b2 = new Precedence(str2);
					}
					if (comm2 == "TEST") {
						b2 = new Test(str2);
					}
				}

				// Starting constructing tree
				if (b1 == 0 && b2 != 0) {
					if (connector == ";") {
						Base* semicom = new Semicolon(new Executable(comm1), b2);
						return constructOrder(con, commands, prectest, semicom);
					}
					
					else if (connector == "&&") {
						Base* ancom = new And(new Executable(comm1), b2);
						return constructOrder(con, commands, prectest, ancom);
					}
					
					else if (connector == "||") {
						Base* orcom = new Or(new Executable(comm1), b2);
						return constructOrder(con, commands, prectest, orcom);
					}
				}
				
				else if (b1 != 0 && b2 == 0) {
					if (connector == ";") {
						Base* semicom = new Semicolon(b1, new Executable(comm2));
						return constructOrder(con, commands, prectest, semicom);
					}
					
					else if (connector == "&&") {
						Base* ancom = new And(b1, new Executable(comm2));
						return constructOrder(con, commands, prectest, ancom);
					}
					
					else if (connector == "||") {
						Base* orcom = new Or(b1, new Executable(comm2));
						return constructOrder(con, commands, prectest, orcom);
					}
				}
				
				else if (b1 != 0 && b2 != 0) {
					if (connector == ";") {
						Base* semicom = new Semicolon(b1, b2);
						return constructOrder(con, commands, prectest, semicom);
					}
					
					else if (connector == "&&") {
						Base* ancom = new And(b1, b2);
						return constructOrder(con, commands, prectest, ancom);
					}
					
					else if (connector == "||") {
						Base* orcom = new Or(b1, b2);
						return constructOrder(con, commands, prectest, orcom);
					}
				}
			}
		}
		cout << "Passed if 3" << endl;
		// If connector queue is empty then there is only one command which will be returned
		return new Executable(commands.front());
	}
}

Base* constructOrder(queue<string> &con, queue<string> &commands, queue<string> &prectest, Base* b) {
	// This is the recursive construction, continues building tree, and returns highest pointer
   
   if (con.empty() || commands.empty() ) {
		return b;
	}
	
	else {
		if (!con.empty()) {
			string connector = con.front();
			con.pop();
			
			string comm = commands.front(); // will get left since right was already made executable
			commands.pop();
			
			// Will build object types based off connectors
			// Uses this if prectest is empty
			if (prectest.empty()) {
            if (connector == ";") {
					Base* semicom = new Semicolon(b, new Executable(comm));
					return constructOrder(con, commands, prectest, semicom);
				}
				
				else if (connector == "&&") {
					Base* ancom = new And(b, new Executable(comm));
					return constructOrder(con, commands, prectest, ancom);
				}
				
				else if (connector == "||") {
					Base* orcom = new Or(b, new Executable(comm));
					return constructOrder(con, commands, prectest, orcom);
				}
			}
			
			else {
				// Initialize variables
				string str = "";

				Base* c = 0;

				// Create appropriate objects
				if (comm == "PREC" || comm == "TEST") {
					string str = prectest.front();
					prectest.pop();
					
					if (comm == "PREC") {
						c = new Precedence(str);
					}
					if (comm == "TEST") {
						c = new Test(str);
					}
				}
				
				// Starting constructing tree
				if (c != 0) {
					if (connector == ";") {
						Base* semicom = new Semicolon(new Executable(comm), c);
						return constructOrder(con, commands, prectest, semicom);
					}
					
					else if (connector == "&&") {
						Base* ancom = new And(new Executable(comm), c);
						return constructOrder(con, commands, prectest, ancom);
					}
					
					else if (connector == "||") {
						Base* orcom = new Or(new Executable(comm), c);
						return constructOrder(con, commands, prectest, orcom);
					}
				}
			}
		}
		
		return b;
	}
}

void connectors(string command, queue<string> &conn) {
  cout << "command connector is :" << command << endl;
  for (size_t i = 0; i < command.size(); i++) {	
  		// Will iterate through string to create connector queue
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
   for (unsigned int i = 0; i < conn.size(); i++){
      cout << "connector queue at " << i << " = " << conn.front() << endl;
   }
}

#include <iostream>
#include <unistd.h>
#include <string>
#include "rshell.h"

int main()
{
	string login = *(getlogin());
	string host = *(gethostname());
	string command;	

	cout << login << "@" << host << "$ ";
	cin >> command;
}

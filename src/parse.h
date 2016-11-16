#ifndef _PARSE_H_
#define _PARSE_H_

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
#include <cstddef>

using namespace std;
using namespace boost;

typedef boost::tokenizer<boost::char_separator<char> > mytok;
typedef mytok::iterator tok_it;

Base* parse(string input);
Base* constructOrder(queue<string> &con, queue<string> &commands, queue<string> &prectest);
Base* constructOrder(queue<string> &con, queue<string> &commands, queue<string> &prectest, Base* b);
void connectors(string command, queue<string> &conn);

#endif

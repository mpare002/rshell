#ifndef ARGUMENT_H
#define ARGUMENT_H

#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include <sstream>
#include <iterator>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

class Base {
   public:
      Base() { };
      virtual ~Base() { };

      virtual bool execute() = 0;
};

class Executable : public Base {
   protected:
      string argument;
   public:
      Executable() { };
      Executable(string arg) : argument(arg) { };
      ~Executable() { };

      string executable;
      bool execute();
};

class And : public Base {
   private:
      Base* leftArg;
      Base* rightArg;
   public:
      And() { };
      And(Base* left, Base* right) : leftArg(left), rightArg(right) { };
      ~And() { };
      bool execute();
};

class Or : public Base {
   private:
      Base* leftArg;
      Base* rightArg;
   public:
      Or() { };
      Or(Base* left, Base* right) : leftArg(left), rightArg(right) { };
      ~Or() { };
      bool execute();
};

class Semicolon : public Base {
   private:
      Base* leftArg;
      Base* rightArg;
   public:
      Semicolon() { };
      Semicolon(Base* left, Base* right) : leftArg(left), rightArg(right) { };
      ~Semicolon() { };
      bool execute();
};

#endif
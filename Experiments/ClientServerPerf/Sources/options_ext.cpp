#include <iostream>
#include <sstream>
#include "options_ext.h"

using std::cout;
using std::endl;
using mlib::OptParser;

std::string getopt_required_string(OptParser &parser, char option)
{
    std::string optArg;
    if (!parser.getopt (option, optArg))
    {
        cout << "Option -" << option << " is missing\n\n";
        cout << "Usage:" << endl;
        cout << parser.synopsis () << endl;
        cout << "Where:" << endl
             << parser.description () << endl;
        exit(1);
    }
    return optArg;
}

int getopt_required_int(OptParser &parser, char option)
{
    std::string optArg;
    if (!parser.getopt (option, optArg))
    {
        cout << "Option -" << option << " is missing\n\n";
        cout << "Usage:" << endl;
        cout << parser.synopsis () << endl;
        cout << "Where:" << endl
             << parser.description () << endl;
        exit(1);
    }
    std::istringstream iss(optArg);
    int val;
    iss >> val;
    if (!iss)
    {
        cout << "Option -" << option << " has incorrect parameter (it is not an integer)\n\n";
        cout << "Option -" << option << " is missing\n\n";
        cout << "Usage:" << endl;
        cout << parser.synopsis () << endl;
        cout << "Where:" << endl
             << parser.description () << endl;
        exit(1);
    }
    return val;
}


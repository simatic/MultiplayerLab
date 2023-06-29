#include <iostream>
#include <sstream>
#include "OptParser_ext.h"

using std::cout;
using std::endl;

namespace mlib {

    OptParser_ext::OptParser_ext (std::initializer_list<const char*> list)
    : OptParser{list}
    {}

    std::string OptParser_ext::getopt_required_string(char option) {
        std::string optArg;
        if (!getopt(option, optArg)) {
            cout << "Option -" << option << " is missing\n\n";
            cout << "Usage:" << endl;
            cout << synopsis() << endl;
            cout << "Where:" << endl
                 << description() << endl;
            exit(1);
        }
        return optArg;
    }

    int OptParser_ext::getopt_required_int(char option) {
        std::string optArg;
        if (!getopt(option, optArg)) {
            cout << "Option -" << option << " is missing\n\n";
            cout << "Usage:" << endl;
            cout << synopsis() << endl;
            cout << "Where:" << endl
                 << description() << endl;
            exit(1);
        }
        std::istringstream iss(optArg);
        int val;
        iss >> val;
        if (!iss) {
            cout << "Option -" << option << " has incorrect parameter (it is not an integer)\n\n";
            cout << "Usage:" << endl;
            cout << synopsis() << endl;
            cout << "Where:" << endl
                 << description() << endl;
            exit(1);
        }
        return val;
    }
}

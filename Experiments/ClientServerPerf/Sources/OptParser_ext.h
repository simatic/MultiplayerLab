// Additional functions to options.h
#pragma once

#include "options.h"

namespace mlib {
    class OptParser_ext: public OptParser
    {
    public:
        OptParser_ext (std::initializer_list<const char*> list);

        std::string getopt_required_string(char option);
        int getopt_required_int(char option);
    };
}

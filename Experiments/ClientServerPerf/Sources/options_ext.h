// Additional functions to options.h
#pragma once

#include "options.h"

std::string getopt_required_string(mlib::OptParser &parser, char option);
int getopt_required_int(mlib::OptParser &parser, char option);
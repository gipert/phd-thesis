#include <string>
#include <vector>
#include <map>

#include "../../../../../../tools/paul_tol_colors.hpp"
using namespace tolcols;

#ifndef GMODELCOMMON_HPP
#define GMODELCOMMON_HPP

std::map<std::string,double> exposure = {
    {"enrBEGe",  32.1},
    {"enrCoax",  28.1},
    {"E1plusE2", 60.2}
};

// how to sum components? CHANGE ME according to the specific results file
std::map<std::string, std::vector<int>> cnames = {
    {"228Ac",      {0, 1, 2, 29, 30, 31, 60, 61, 62}},
    {"212Bi208Tl", {3, 4, 5, 32, 33, 34, 63, 64, 65}},
    {"214Bi214Pb", {6, 7, 8, 9, 10, 11, 35, 36, 37, 38, 39, 66, 67, 68, 69, 70}},
    {"60Co",       {12, 40, 71}},
    {"40K",        {13, 14, 15, 16, 17, 18, 19, 20, 21, 41, 42, 43, 44, 45, 46, 47, 48, 49, 72, 73, 74, 75, 76, 77, 78, 79, 80}},
    {"42K",        {22, 23, 24, 25, 50, 51, 52, 81, 82, 83}},
    {"2nbb",       {28, 58, 59}},
    {"alpha",      {55, 56, 57, 86, 87, 88}}
};

// assign colors
std::map<std::string,int> hcolor = {
    {"228Ac",      kTVibBlue},
    {"212Bi208Tl", kTVibCyan},
    {"214Bi214Pb", kTVibTeal},
    {"60Co",       kTVibOrange},
    {"40K",        kTVibRed},
    {"42K",        kTVibMagenta},
    {"2nbb",       kTMutOlive},
    {"alpha",      kTMutSand}
};

// assign nice names
std::map<std::string,std::string> nice_names = {
    {"228Ac",      "^{228}Ac"},
    {"212Bi208Tl", "^{212}Bi + ^{208}Tl"},
    {"214Bi214Pb", "^{214}Bi + ^{214}Pb"},
    {"60Co",       "^{60}Co"},
    {"40K",        "^{40}K"},
    {"42K",        "^{42}K"},
    {"2nbb",       "2#nu#beta#beta"},
    {"alpha",      "^{210}Po"}
};

// dictionary for datasets
std::map<int,std::string> datdict = {
    {0, "E1plusE2"},
    {1, "enrBEGe"},
    {2, "enrCoax"},
};

#endif

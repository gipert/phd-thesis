#include <string>
#include <vector>
#include <map>
#include "../../../../../../tools/paul_tol_colors.hpp"
using namespace tolcols;

#ifndef KMODELCOMMON_HPP
#define KMODELCOMMON_HPP

// list of possible histogram names in the results file
std::vector<std::string> cnames = {
    "K40Cables", "K40Holders", "K40Minishroud", "K40Minishroud_copy",
    "K40Fibers", "K40SiPM", "K40CopperShroud", "K40CopperShroud_copy", "K40CC3",
    "K42OutsideMS", "K42LArAbove", "K42NplusCoax", "K42NplusBEGe",
    "2nbb",
    "Bi214Cables", "Bi214Holders"
};

// list of possible isotopes
std::vector<std::string> inames = {
    "K40", "K42", "2nbb", "Bi212", "Bi214", "Tl208", "Pb214",
    "Pa234m", "Bi207"
};

// how to merge components
std::map<std::string, std::vector<std::string>> cmerge = {
    {"K40Near",  {"K40Cables", "K40Holders", "K40Minishroud", "K40Minishroud_copy"}},
    {"K40Far",   {"K40Fibers", "K40SiPM", "K40CopperShroud", "K40CopperShroud_copy", "K40CC3"}},
    {"K42Nplus", {"K42NplusCoax", "K42NplusBEGe"}}
};

// assign colors
std::map<std::string,int> hcolor = {
    {"K40",                 kTBriBlue},
    {"K40Near",             kTBriCyan},
    {"K40Far",              kTBriBlue},

    {"K42",                 kTBriRed},
    {"2nbb",                kTBriGreen},
    {"Bi214",               kTBriPurple},

    {"K40Cables",           kTMutIndigo},
    {"K40Holders",          kTBriYellow},
    {"K40Minishroud",       kTMutTeal},
    {"K40Fibers",           kTMutOlive},
    {"K40SiPM",             kTMutSand},
    {"K40CopperShroud",     kTMutRose},
    {"K40CC3",              kTMutPurple},

    {"K42LAr",              kTVibBlue},
    {"K42OutsideMS",        kTVibBlue},
    {"K42LArAbove",         kTVibMagenta},
    {"K42NplusCoax",        kTVibGrey},
    {"K42NplusBEGe",        kTVibOrange},

    {"K42Nplus",            kTVibOrange}
};

// assign nice names
std::map<std::string,std::string> nice_names = {
    {"K40Near", "^{40}K close"},
    {"K40Far",  "^{40}K far"},

    {"K40",   "^{40}K"},
    {"K42",   "^{42}K"},
    {"2nbb",  "2#nu#beta#beta"},
    {"Bi214", "^{214}Bi"},

    {"K40Cables",           "^{40}K - flat cables"},
    {"K40Holders",          "^{40}K - holders"},
    {"K40Minishroud",       "^{40}K - mini-sh"},
    {"K40Fibers",           "^{40}K - fiber-sh"},
    {"K40SiPM",             "^{40}K - SiPM"},
    {"K40CopperShroud",     "^{40}K - copper-sh"},
    {"K40CC3",              "^{40}K - CC3"},

    {"K42LAr",              "^{42}K - LAr"},
    {"K42OutsideMS",        "^{42}K - LAr"},
    {"K42LArAbove",         "^{42}K - LAr above"},
    {"K42NplusCoax",        "^{42}K - n^{+} (Coax)"},
    {"K42NplusBEGe",        "^{42}K - n^{+} (BEGe)"},

    {"K42Nplus",        "^{42}K - n^{+} cont."}
};

// detector names
std::vector<std::string> det = {
  "GD91A", "GD35B", "GD02B",  "GD00B", "GD61A", "GD89B", /*"GD02D",*/
  "GD91C", "ANG5",  "RG1",    "ANG3",  "GD02A", "GD32B", "GD32A",
  "GD32C", "GD89C", "GD61C",  "GD76B", "GD00C", "GD35C", "GD76C",
  "GD89D", "GD00D", "GD79C",  "GD35A", "GD91B", "GD61B", "ANG2" ,
  "RG2",   "ANG4",  "GD00A",  "GD02C", "GD79B", "GD91D", "GD32D",
  "GD89A", "ANG1",  "GTF112", "GTF32", "GTF45"
};

// dictionary for datasets
std::map<int,std::string> datdict = {
    {0, "#font[102]{M1-K40}"},
    {1, "#font[102]{M1-K42}"},
    {2, "#font[102]{M1-SB1}"},
    {3, "#font[102]{M1-SB2}"},
    {4, "#font[102]{M1-SB3}"},
    {5, "#font[102]{M2-K40}"},
    {6, "#font[102]{M2-K42}"},
    {7, "#font[102]{M2-SB1}"},
    {8, "#font[102]{M2-SB2}"},
    {9, "#font[102]{M2-SB3}"}
};

#endif

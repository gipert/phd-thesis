#include <vector>
#include <map>
#include <string>

#include "../../../../../../tools/paul_tol_colors.hpp"
using namespace tolcols;

#ifndef KMODELCOMMONCOMPLETE_HPP
#define KMODELCOMMONCOMPLETE_HPP

// list of possible histogram names in the results file
std::vector<std::string> cnames = {
    "K40Cables", "K40Holders",
    "K40Minishroud", "K40Minishroud1", "K40Minishroud2", "K40Minishroud3", "K40Minishroud4",
    "K40Minishroud5", "K40Minishroud6", "K40Minishroud7", "K40MinishroudTop7",
    "K40Fibers", "K40SiPM", "K40Tetratex", "K40Tetratex_copy", "K40CC3",
    "K42OutsideMS", "K42LArAboveMS1", "K42LArAboveMS2", "K42LArAboveMS3", "K42LArAboveMS4",
    "K42LArAboveMS5", "K42LArAboveMS6", "K42LArAboveMS7",
    "K42NplusCoax", "K42NplusBEGe",
    "2nbb",
    "Bi214Cables", "Bi214Holders"
};

// names of asymmetric K40 components
std::vector<std::string> acnames_k40 = {
    "K40Minishroud1", "K40Minishroud2", "K40Minishroud3", "K40Minishroud4",
    "K40Minishroud5", "K40Minishroud6", "K40Minishroud7", "K40MinishroudTop7"
};

// names of asymmetric K42 LAr (AA) components
std::vector<std::string> acnames_k42 = {
    "K42LArAboveMS1", "K42LArAboveMS2", "K42LArAboveMS3", "K42LArAboveMS4",
    "K42LArAboveMS5", "K42LArAboveMS6", "K42LArAboveMS7"
};

// list of possible isotopes
std::vector<std::string> inames = {
    "K40", "K42", "2nbb", "Bi212", "Bi214", "Tl208", "Pb214",
    "Pa234m", "Bi207"
};

// how to merge components
std::map<std::string, std::vector<std::string>> cmerge = {
    {"K40Near",    {"K40Cables", "K40Holders", "K40Minishroud", "K40Minishroud1", "K40Minishroud2", "K40Minishroud3",
                    "K40Minishroud4", "K40Minishroud5", "K40Minishroud6", "K40Minishroud7", "K40MinishroudTop7"}},
    {"K40Far",     {"K40Fibers", "K40SiPM", "K40Tetratex", "K40Tetratex_copy", "K40CC3"}},
    {"K42Nplus",   {"K42NplusCoax", "K42NplusBEGe"}},
    {"K42LAr",     {"K42OutsideMS"}},
    {"K42LArAbove",{"K42LArAboveMS1", "K42LArAboveMS2", "K42LArAboveMS3", "K42LArAboveMS4",
                    "K42LArAboveMS5", "K42LArAboveMS6", "K42LArAboveMS7"}}
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
    {"K42LArAbove",         kTVibMagenta},
    {"K42NplusCoax",        kTVibGrey},
    {"K42NplusBEGe",        kTVibOrange},

    {"K42Nplus",            kTVibOrange}
};

// assign nice names
std::map<std::string,std::string> nice_names = {
    {"K40Near", "^{40}K near"},
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
    {"K42LArAbove",         "^{42}K - LAr (AA)"},
    {"K42NplusCoax",        "^{42}K - n^{+} (Coax)"},
    {"K42NplusBEGe",        "^{42}K - n^{+} (BEGe)"},

    {"K42Nplus",        "^{42}K - n^{+}"}
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
    {0, "single-det ^{40}K line"},
    {1, "single-det ^{42}K line"},
    {2, "single-det Band 1"},
    {3, "single-det Band 2"},
    {4, "single-det Band 3"},
    {5, "two-det ^{40}K line"},
    {6, "two-det ^{42}K line"},
    {7, "two-det Band 1"},
    {8, "two-det Band 2"},
    {9, "two-det Band 3"}
};

#endif

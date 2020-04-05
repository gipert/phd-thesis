void amodel_common() {

    /*std::map<std::string,double>*/
        exposure = {{"enrBEGe", 32.1},
                    {"enrCoax", 28.1},
                    {"natCoax",  9.1}};

    // possible components names
    /* std::vector<std::string> */ cnames = {
        "Po210_pPlus_400nm_p0c0",
        "Po210_pPlus_500nm_p1c0",
        "Po210_pPlus_600nm_p2c0",
        "Ra226chain_pPlus_500nm_p3c0",
        "offset_p4c0", "slope_p5c0",

        "Po210_pPlus_300nm_p0c0",
        "Po210_pPlus_400nm_p1c0",
        "Po210_pPlus_500nm_p2c0",
        "Po210_pPlus_600nm_p3c0",
        "Po210_pPlus_700nm_p4c0",
        "Ra226chain_pPlus_100nm_p5c0",
        "offset_p6c0", "slope_p7c0",

        "Po210_pPlus_200nm_p0c0",
        "Po210_pPlus_300nm_p1c0",
        "Po210_pPlus_400nm_p2c0",
        "Po210_pPlus_500nm_p3c0",
        "Po210_pPlus_600nm_p4c0",
        "Ra226chain_pPlus_300nm_p5c0",
        "offset_p7c0", "slope_p8c0"
    };

    // assign colors
    /* std::map<std::string,int> */ hcolor = {
        {"Po210_pPlus_400nm_p0c0",    PTColor["kTBriBlue"]},
        {"Po210_pPlus_500nm_p1c0",    PTColor["kTBriRed"]},
        {"Po210_pPlus_600nm_p2c0",    PTColor["kTBriGreen"]},
        {"Ra226chain_pPlus_500nm_p3c0", PTColor["kTBriPurple"]},
        {"offset_p4c0",               PTColor["kTBriCyan"]},
        {"slope_p5c0",                PTColor["kTBriYellow"]},

        {"Po210_pPlus_300nm_p0c0",    PTColor["kTVibOrange"]},
        {"Po210_pPlus_400nm_p1c0",    PTColor["kTBriBlue"]},
        {"Po210_pPlus_500nm_p2c0",    PTColor["kTBriRed"]},
        {"Po210_pPlus_600nm_p3c0",    PTColor["kTBriGreen"]},
        {"Po210_pPlus_700nm_p4c0",    PTColor["kTVibMagenta"]},
        {"Ra226chain_pPlus_100nm_p5c0", PTColor["kTBriPurple"]},
        {"offset_p6c0",               PTColor["kTBriCyan"]},
        {"slope_p7c0",                PTColor["kTBriYellow"]},

        {"Po210_pPlus_200nm_p0c0",    PTColor["kTLigPink"]},
        {"Po210_pPlus_300nm_p1c0",    PTColor["kTVibOrange"]},
        {"Po210_pPlus_400nm_p2c0",    PTColor["kTBriBlue"]},
        {"Po210_pPlus_500nm_p3c0",    PTColor["kTBriRed"]},
        {"Po210_pPlus_600nm_p4c0",    PTColor["kTBriGreen"]},
        {"Ra226chain_pPlus_300nm_p5c0", PTColor["kTBriPurple"]},
        {"offset_p6c0",               PTColor["kTBriCyan"]},
        {"slope_p7c0",                PTColor["kTBriYellow"]}
    };

    // assign nice names
    /* std::map<std::string,std::string> */ nice_names = {
        {"Po210_pPlus_400nm_p0c0",    "^{210}Po - p^{+} (400 nm)"},
        {"Po210_pPlus_500nm_p1c0",    "^{210}Po - p^{+} (500 nm)"},
        {"Po210_pPlus_600nm_p2c0",    "^{210}Po - p^{+} (600 nm)"},
        {"Ra226chain_pPlus_500nm_p3c0", "^{226}Ra chain - p^{+} (500 nm)"},
        {"offset_p4c0",               "energy-degraded"},
        {"slope_p5c0",                "y = p_{1}E"},

        {"Po210_pPlus_300nm_p0c0",    "^{210}Po - p^{+} (300 nm)"},
        {"Po210_pPlus_400nm_p1c0",    "^{210}Po - p^{+} (400 nm)"},
        {"Po210_pPlus_500nm_p2c0",    "^{210}Po - p^{+} (500 nm)"},
        {"Po210_pPlus_600nm_p3c0",    "^{210}Po - p^{+} (600 nm)"},
        {"Po210_pPlus_700nm_p4c0",    "^{210}Po - p^{+} (700 nm)"},
        {"Ra226chain_pPlus_100nm_p5c0", "^{226}Ra chain - p^{+} (100 nm)"},
        {"offset_p6c0",               "energy-degraded"},
        {"slope_p7c0",                "y = p_{1}E"},

        {"Po210_pPlus_200nm_p0c0",    "^{210}Po - p^{+} (200 nm)"},
        {"Po210_pPlus_300nm_p1c0",    "^{210}Po - p^{+} (300 nm)"},
        {"Po210_pPlus_400nm_p2c0",    "^{210}Po - p^{+} (400 nm)"},
        {"Po210_pPlus_500nm_p3c0",    "^{210}Po - p^{+} (500 nm)"},
        {"Po210_pPlus_600nm_p4c0",    "^{210}Po - p^{+} (600 nm)"},
        {"Ra226chain_pPlus_300nm_p5c0", "^{226}Ra chain - p^{+} (300 nm)"},
        {"offset_p6c0",               "energy-degraded"},
        {"slope_p7c0",                "y = p_{1}E"}
    };

    // detector names
    /* std::vector<std::string> */ det = {
      "GD91A", "GD35B", "GD02B",  "GD00B", "GD61A", "GD89B", "GD02D",
      "GD91C", "ANG5",  "RG1",    "ANG3",  "GD02A", "GD32B", "GD32A",
      "GD32C", "GD89C", "GD61C",  "GD76B", "GD00C", "GD35C", "GD76C",
      "GD89D", "GD00D", "GD79C",  "GD35A", "GD91B", "GD61B", "ANG2" ,
      "RG2",   "ANG4",  "GD00A",  "GD02C", "GD79B", "GD91D", "GD32D",
      "GD89A", "ANG1",  "GTF112", "GTF32", "GTF45_2"
    };

    // dictionary for datasets
    /* std::map<int,std::string> */ datdict = {
        {0, "enrBEGe"},
        {1, "enrCoax"},
        {2, "natCoax"},
    };
}

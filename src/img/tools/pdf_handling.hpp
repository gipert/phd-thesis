// tools to handle pdfs

#ifndef PDFHANDLING_HPP
#define PDFHANDLING_HPP

struct pdf_set {
    // TH1D* M1_ch0;
    // TH1D* M1_ch1;
    // TH1D* M1_ch2;
    // TH1D* M1_ch3;
    // TH1D* M1_ch4;
    // TH1D* M1_ch5;
    // TH1D* M1_ch6;
    // TH1D* M1_ch7;
    // TH1D* M1_ch8;
    // TH1D* M1_ch9;
    // TH1D* M1_ch10;
    // TH1D* M1_ch11;
    // TH1D* M1_ch12;
    // TH1D* M1_ch13;
    // TH1D* M1_ch14;
    // TH1D* M1_ch15;
    // TH1D* M1_ch16;
    // TH1D* M1_ch17;
    // TH1D* M1_ch18;
    // TH1D* M1_ch19;
    // TH1D* M1_ch20;
    // TH1D* M1_ch21;
    // TH1D* M1_ch22;
    // TH1D* M1_ch23;
    // TH1D* M1_ch24;
    // TH1D* M1_ch25;
    // TH1D* M1_ch26;
    // TH1D* M1_ch27;
    // TH1D* M1_ch28;
    // TH1D* M1_ch29;
    // TH1D* M1_ch30;
    // TH1D* M1_ch31;
    // TH1D* M1_ch32;
    // TH1D* M1_ch33;
    // TH1D* M1_ch34;
    // TH1D* M1_ch35;
    // TH1D* M1_ch36;
    // TH1D* M1_ch37;
    // TH1D* M1_ch38;
    // TH1D* M1_ch39;
    TH1D* M1_enrBEGe;
    TH1D* M1_enrCoax;
    TH1D* M1_natCoax;
    TH1D* M1_all_1525;
    TH1D* M1_all_1461;
    TH1D* M1_all_full;
    TH1D* M1_all_S1;
    TH1D* M1_all_S2;
    TH1D* M1_all_S3;
    TH2D* M2_enrE1vsE2;
    TH1D* M2_enrE1plusE2;
    TH1D* M2_enrE1andE2;
    TH2D* M2_ID1vsID2_1525;
    TH2D* M2_ID1vsID2_1461;
    TH2D* M2_ID1vsID2_full;
    TH2D* M2_ID1vsID2_S1;
    TH2D* M2_ID1vsID2_S2;
    TH2D* M2_ID1vsID2_S3;
    TH1D* M2_ID1andID2_1525;
    TH1D* M2_ID1andID2_1461;
    TH1D* M2_ID1andID2_full;
    TH1D* M2_ID1andID2_S1;
    TH1D* M2_ID1andID2_S2;
    TH1D* M2_ID1andID2_S3;

    Long64_t nedep;
    Long64_t ncoin;

    TFile* file;
};

pdf_set get_pdf_set(std::string volume, std::string part, std::string isotope, bool norm = true) {
    pdf_set set;
    auto file = new TFile(("./gerda-pdfs/" + volume + "/" + part + "/" + isotope
                           + "/pdf-" + volume + "-" + part + "-" + isotope + ".root").c_str(), "READ");

    if (file->IsZombie()) {
        throw std::runtime_error("Could not find PDF files. Did you put the PDFs under ./gerda-pdfs/ ?");
    }

    // set.M1_ch0            = dynamic_cast<TH1D*>(file->Get("M1_ch0"));
    // set.M1_ch1            = dynamic_cast<TH1D*>(file->Get("M1_ch1"));
    // set.M1_ch2            = dynamic_cast<TH1D*>(file->Get("M1_ch2"));
    // set.M1_ch3            = dynamic_cast<TH1D*>(file->Get("M1_ch3"));
    // set.M1_ch4            = dynamic_cast<TH1D*>(file->Get("M1_ch4"));
    // set.M1_ch5            = dynamic_cast<TH1D*>(file->Get("M1_ch5"));
    // set.M1_ch6            = dynamic_cast<TH1D*>(file->Get("M1_ch6"));
    // set.M1_ch7            = dynamic_cast<TH1D*>(file->Get("M1_ch7"));
    // set.M1_ch8            = dynamic_cast<TH1D*>(file->Get("M1_ch8"));
    // set.M1_ch9            = dynamic_cast<TH1D*>(file->Get("M1_ch9"));
    // set.M1_ch10           = dynamic_cast<TH1D*>(file->Get("M1_ch10"));
    // set.M1_ch11           = dynamic_cast<TH1D*>(file->Get("M1_ch11"));
    // set.M1_ch12           = dynamic_cast<TH1D*>(file->Get("M1_ch12"));
    // set.M1_ch13           = dynamic_cast<TH1D*>(file->Get("M1_ch13"));
    // set.M1_ch14           = dynamic_cast<TH1D*>(file->Get("M1_ch14"));
    // set.M1_ch15           = dynamic_cast<TH1D*>(file->Get("M1_ch15"));
    // set.M1_ch16           = dynamic_cast<TH1D*>(file->Get("M1_ch16"));
    // set.M1_ch17           = dynamic_cast<TH1D*>(file->Get("M1_ch17"));
    // set.M1_ch18           = dynamic_cast<TH1D*>(file->Get("M1_ch18"));
    // set.M1_ch19           = dynamic_cast<TH1D*>(file->Get("M1_ch19"));
    // set.M1_ch20           = dynamic_cast<TH1D*>(file->Get("M1_ch20"));
    // set.M1_ch21           = dynamic_cast<TH1D*>(file->Get("M1_ch21"));
    // set.M1_ch22           = dynamic_cast<TH1D*>(file->Get("M1_ch22"));
    // set.M1_ch23           = dynamic_cast<TH1D*>(file->Get("M1_ch23"));
    // set.M1_ch24           = dynamic_cast<TH1D*>(file->Get("M1_ch24"));
    // set.M1_ch25           = dynamic_cast<TH1D*>(file->Get("M1_ch25"));
    // set.M1_ch26           = dynamic_cast<TH1D*>(file->Get("M1_ch26"));
    // set.M1_ch27           = dynamic_cast<TH1D*>(file->Get("M1_ch27"));
    // set.M1_ch28           = dynamic_cast<TH1D*>(file->Get("M1_ch28"));
    // set.M1_ch29           = dynamic_cast<TH1D*>(file->Get("M1_ch29"));
    // set.M1_ch30           = dynamic_cast<TH1D*>(file->Get("M1_ch30"));
    // set.M1_ch31           = dynamic_cast<TH1D*>(file->Get("M1_ch31"));
    // set.M1_ch32           = dynamic_cast<TH1D*>(file->Get("M1_ch32"));
    // set.M1_ch33           = dynamic_cast<TH1D*>(file->Get("M1_ch33"));
    // set.M1_ch34           = dynamic_cast<TH1D*>(file->Get("M1_ch34"));
    // set.M1_ch35           = dynamic_cast<TH1D*>(file->Get("M1_ch35"));
    // set.M1_ch36           = dynamic_cast<TH1D*>(file->Get("M1_ch36"));
    // set.M1_ch37           = dynamic_cast<TH1D*>(file->Get("M1_ch37"));
    // set.M1_ch38           = dynamic_cast<TH1D*>(file->Get("M1_ch38"));
    // set.M1_ch39           = dynamic_cast<TH1D*>(file->Get("M1_ch39"));
    set.M1_enrBEGe        = dynamic_cast<TH1D*>(file->Get("M1_enrBEGe"));
    set.M1_enrCoax        = dynamic_cast<TH1D*>(file->Get("M1_enrCoax"));
    set.M1_natCoax        = dynamic_cast<TH1D*>(file->Get("M1_natCoax"));
    set.M1_all_1525       = dynamic_cast<TH1D*>(file->Get("M1_all_1525"));
    set.M1_all_1461       = dynamic_cast<TH1D*>(file->Get("M1_all_1461"));
    set.M1_all_full       = dynamic_cast<TH1D*>(file->Get("M1_all_full"));
    set.M1_all_S1         = dynamic_cast<TH1D*>(file->Get("M1_all_S1"));
    set.M1_all_S2         = dynamic_cast<TH1D*>(file->Get("M1_all_S2"));
    set.M1_all_S3         = dynamic_cast<TH1D*>(file->Get("M1_all_S3"));
    set.M2_enrE1vsE2      = dynamic_cast<TH2D*>(file->Get("M2_enrE1vsE2"));
    set.M2_enrE1plusE2    = dynamic_cast<TH1D*>(file->Get("M2_enrE1plusE2"));
    set.M2_enrE1andE2     = dynamic_cast<TH1D*>(file->Get("M2_enrE1andE2"));
    set.M2_ID1vsID2_1525  = dynamic_cast<TH2D*>(file->Get("M2_ID1vsID2_1525"));
    set.M2_ID1vsID2_1461  = dynamic_cast<TH2D*>(file->Get("M2_ID1vsID2_1461"));
    set.M2_ID1vsID2_full  = dynamic_cast<TH2D*>(file->Get("M2_ID1vsID2_full"));
    set.M2_ID1vsID2_S1    = dynamic_cast<TH2D*>(file->Get("M2_ID1vsID2_S1"));
    set.M2_ID1vsID2_S2    = dynamic_cast<TH2D*>(file->Get("M2_ID1vsID2_S2"));
    set.M2_ID1vsID2_S3    = dynamic_cast<TH2D*>(file->Get("M2_ID1vsID2_S3"));
    set.M2_ID1andID2_1525 = dynamic_cast<TH1D*>(file->Get("M2_ID1andID2_1525"));
    set.M2_ID1andID2_1461 = dynamic_cast<TH1D*>(file->Get("M2_ID1andID2_1461"));
    set.M2_ID1andID2_full = dynamic_cast<TH1D*>(file->Get("M2_ID1andID2_full"));
    set.M2_ID1andID2_S1   = dynamic_cast<TH1D*>(file->Get("M2_ID1andID2_S1"));
    set.M2_ID1andID2_S2   = dynamic_cast<TH1D*>(file->Get("M2_ID1andID2_S2"));
    set.M2_ID1andID2_S3   = dynamic_cast<TH1D*>(file->Get("M2_ID1andID2_S3"));

    set.file = file;

    double n_edep = dynamic_cast<TParameter<Long64_t>*>(file->Get("NumberOfPrimariesEdep"))->GetVal();
    double n_coin = dynamic_cast<TParameter<Long64_t>*>(file->Get("NumberOfPrimariesCoin")) ?
                    dynamic_cast<TParameter<Long64_t>*>(file->Get("NumberOfPrimariesCoin"))->GetVal() : 0;

    if (norm) {
        // set.M1_ch0            ->Scale(1./n_edep);
        // set.M1_ch1            ->Scale(1./n_edep);
        // set.M1_ch2            ->Scale(1./n_edep);
        // set.M1_ch3            ->Scale(1./n_edep);
        // set.M1_ch4            ->Scale(1./n_edep);
        // set.M1_ch5            ->Scale(1./n_edep);
        // set.M1_ch6            ->Scale(1./n_edep);
        // set.M1_ch7            ->Scale(1./n_edep);
        // set.M1_ch8            ->Scale(1./n_edep);
        // set.M1_ch9            ->Scale(1./n_edep);
        // set.M1_ch10           ->Scale(1./n_edep);
        // set.M1_ch11           ->Scale(1./n_edep);
        // set.M1_ch12           ->Scale(1./n_edep);
        // set.M1_ch13           ->Scale(1./n_edep);
        // set.M1_ch14           ->Scale(1./n_edep);
        // set.M1_ch15           ->Scale(1./n_edep);
        // set.M1_ch16           ->Scale(1./n_edep);
        // set.M1_ch17           ->Scale(1./n_edep);
        // set.M1_ch18           ->Scale(1./n_edep);
        // set.M1_ch19           ->Scale(1./n_edep);
        // set.M1_ch20           ->Scale(1./n_edep);
        // set.M1_ch21           ->Scale(1./n_edep);
        // set.M1_ch22           ->Scale(1./n_edep);
        // set.M1_ch23           ->Scale(1./n_edep);
        // set.M1_ch24           ->Scale(1./n_edep);
        // set.M1_ch25           ->Scale(1./n_edep);
        // set.M1_ch26           ->Scale(1./n_edep);
        // set.M1_ch27           ->Scale(1./n_edep);
        // set.M1_ch28           ->Scale(1./n_edep);
        // set.M1_ch29           ->Scale(1./n_edep);
        // set.M1_ch30           ->Scale(1./n_edep);
        // set.M1_ch31           ->Scale(1./n_edep);
        // set.M1_ch32           ->Scale(1./n_edep);
        // set.M1_ch33           ->Scale(1./n_edep);
        // set.M1_ch34           ->Scale(1./n_edep);
        // set.M1_ch35           ->Scale(1./n_edep);
        // set.M1_ch36           ->Scale(1./n_edep);
        // set.M1_ch37           ->Scale(1./n_edep);
        // set.M1_ch38           ->Scale(1./n_edep);
        // set.M1_ch39           ->Scale(1./n_edep);
        set.M1_enrBEGe        ->Scale(1./n_edep);
        set.M1_enrCoax        ->Scale(1./n_edep);
        set.M1_natCoax        ->Scale(1./n_edep);
        set.M1_all_1525       ->Scale(1./n_edep);
        set.M1_all_1461       ->Scale(1./n_edep);
        set.M1_all_full       ->Scale(1./n_edep);
        set.M1_all_S1         ->Scale(1./n_edep);
        set.M1_all_S2         ->Scale(1./n_edep);
        set.M1_all_S3         ->Scale(1./n_edep);
        if (n_coin > 0) {
            set.M2_enrE1vsE2     ->Scale(1./n_coin);
            set.M2_enrE1plusE2   ->Scale(1./n_coin);
            set.M2_enrE1andE2    ->Scale(1./n_coin);
            set.M2_ID1vsID2_1525 ->Scale(1./n_coin);
            set.M2_ID1vsID2_1461 ->Scale(1./n_coin);
            set.M2_ID1vsID2_full ->Scale(1./n_coin);
            set.M2_ID1vsID2_S1   ->Scale(1./n_coin);
            set.M2_ID1vsID2_S2   ->Scale(1./n_coin);
            set.M2_ID1vsID2_S3   ->Scale(1./n_coin);
            set.M2_ID1andID2_1525->Scale(1./n_coin);
            set.M2_ID1andID2_1461->Scale(1./n_coin);
            set.M2_ID1andID2_full->Scale(1./n_coin);
            set.M2_ID1andID2_S1  ->Scale(1./n_coin);
            set.M2_ID1andID2_S2  ->Scale(1./n_coin);
            set.M2_ID1andID2_S3  ->Scale(1./n_coin);
        }
    }

    set.nedep = n_edep;
    set.ncoin = n_coin;

    return set;
}

#endif

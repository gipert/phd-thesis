void process(std::ofstream& f, TH1* h) {
    f << "lowedge, bincontent" << std::endl;
    f << "0, 0" << std::endl;
    for (int i = 1; i <= h->GetNbinsX()+1; ++i) {
        f << h->GetBinLowEdge(i) << ", " << h->GetBinContent(i) << std::endl;
    }
    f << "6000, 0" << std::endl;
}

void csv() {
    TH1* h;
    std::ofstream f;

    TFile* fin = TFile::Open("gerda-data-bkgmodel-all-v06.13-raw.bb-p0_0_050000.root");

    // h = (TH1*)fin->Get("M1_enrBEGe");
    // f.open("bb-bege-raw.csv");
    // process(f, h);
    // f.close();

    // h = (TH1*)fin->Get("M1_enrCoax");
    // f.open("bb-scoax-raw.csv");
    // process(f, h);
    // f.close();

    // h = (TH1*)fin->Get("M1_invCoax");
    // f.open("bb-icoax-raw.csv");
    // process(f, h);
    // f.close();

    h = (TH1*)fin->Get("M1_enrAll");
    f.open("bb-all-raw.csv");
    process(f, h);
    f.close();

    fin->Close();

    fin = TFile::Open("gerda-data-bkgmodel-all-v06.13-lar.bb-p0_0_050000.root");

    // h = (TH1*)fin->Get("M1_enrBEGe");
    // f.open("bb-bege-lar.csv");
    // process(f, h);
    // f.close();

    // h = (TH1*)fin->Get("M1_enrCoax");
    // f.open("bb-scoax-lar.csv");
    // process(f, h);
    // f.close();

    // h = (TH1*)fin->Get("M1_invCoax");
    // f.open("bb-icoax-lar.csv");
    // process(f, h);
    // f.close();

    h = (TH1*)fin->Get("M1_enrAll");
    f.open("bb-all-lar.csv");
    process(f, h);
    f.close();

    fin->Close();

    fin = TFile::Open("gerda-data-bkgmodel-all-v06.13-lar_psd.bb-p0_0_050000.root");

    // h = (TH1*)fin->Get("M1_enrBEGe");
    // f.open("bb-bege-lar_psd.csv");
    // process(f, h);
    // f.close();

    // h = (TH1*)fin->Get("M1_enrCoax");
    // f.open("bb-scoax-lar_psd.csv");
    // process(f, h);
    // f.close();

    // h = (TH1*)fin->Get("M1_invCoax");
    // f.open("bb-icoax-lar_psd.csv");
    // process(f, h);
    // f.close();

    h = (TH1*)fin->Get("M1_enrAll");
    f.open("bb-all-lar_psd.csv");
    process(f, h);
    f.close();

    fin->Close();
}

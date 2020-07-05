double exposure = 103.686;
int nbins = 290;
int rangelow = 989;
int rangeup = 5339;
// int nbins = 8000;
// int rangelow = 0;
// int rangeup = 8000;

struct dataset {
    int id;
    std::string name;
    double exposure;
    TH1* hist;
};

void root2csv() {

    auto summary = TFile::Open("gerda-summary-run0053-run0114-phy-slim.root");

    std::vector<dataset> datasets = {
        {0, "EnrBEGe",    31.460, nullptr},
        {1, "EnrCoax",    28.638, nullptr},
        {5, "EnrCoax_2",  13.197, nullptr},
        {6, "EnrInvCoax",  8.589, nullptr},
        {8, "EnrBEGe_2",  21.865, nullptr}
    };

    for (auto cut : std::vector<std::string>{"hRaw", "hLAr", "hPSD", "hPSDLAr"}) {

        std::ofstream fout(cut + "-15keV.csv");
        fout << "energy, ";

        for (auto it = datasets.begin(); it != datasets.end(); ++it) {
            auto raw = (TH1D*)summary->Get((cut + std::to_string(it->id)).c_str());
            it->hist = new TH1D(Form("%s_proj", raw->GetName()), raw->GetTitle(), nbins, rangelow, rangeup);
            for (int inBin = 0; inBin <= raw->GetNbinsX(); inBin++) {
                int outBin = it->hist->FindBin(raw->GetBinCenter(inBin));
                it->hist->SetBinContent(
                    outBin,
                    it->hist->GetBinContent(outBin) +
                    raw->GetBinContent(inBin)*raw->GetBinWidth(inBin)/it->hist->GetBinWidth(outBin)
                );
            }
            fout << it->name << ", ";
            it->hist->Scale(1/exposure);
        }
        fout << "EnrAll\n";

        for (int i = 1; i <= datasets[0].hist->GetNbinsX(); ++i) {

            fout << datasets[0].hist->GetXaxis()->GetBinLowEdge(i) << ", ";

            double sum = 0;

            for (auto it = datasets.begin(); it != datasets.end(); ++it) {
                sum += it->hist->GetBinContent(i);
                fout << it->hist->GetBinContent(i) << ", ";
            }

            fout << sum << std::endl;
        }
        fout.close();
    }
}

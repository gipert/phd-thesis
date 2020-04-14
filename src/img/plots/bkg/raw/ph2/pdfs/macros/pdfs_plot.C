/* pdfs_plot.C
 *
 * Author: Luigi Pertoldi - pertoldi@pd.infn.it
 * Created: Tue 1 Jan 2019
 *
 */

#include "../../../../../../tools/bayesian_blocks.hpp"
#include "../../../../../../tools/pdf_handling.hpp"
#include "../../results/kmodel/kmodel_common.hpp"

// color alpha
double f_alpha = 0.05;

// line width of kmodel plots
int lw = 1;

// use bayesian blocks algorithm
bool use_bb = true;
int rebin = 10;

auto freec = 10000;

TH1D* _big(TH1D* h);
TH1D* remove_bin(TH1D* h);
TH1* get_or_create_bb(TH1*);
void compute_and_save(TH1D* h, double scale, std::string name, bool force = false);
void compute_bayes_blocks();

void a_plot_Po210() {

    TFile tfile("./gerda-pdfs/alphas/pdf-Po210-pPlus.root", "READ");
    std::vector<int> len = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    std::vector<TH1D*> h;
    for (auto& l : len) {
        h.push_back(dynamic_cast<TH1D*>(tfile.Get(("hist_dl" + std::to_string(l) + "nm").c_str())));
      h.back()->Scale(1E-9); // normalize to primaries
    }

    // props
    h.back()->GetXaxis()->SetRangeUser(3500, 5500);
    h.back()->GetYaxis()->SetRangeUser(1.1E-6, 5E-1);
    h.back()->SetTitle(";energy [keV];probability / (decay keV)");

    auto c = new TCanvas();
    auto leg = new TLegend(0.1, 0.6, 0.75, 0.95);
    leg->SetHeader("^{210}Po on p^{+}", "L");
    leg->SetNColumns(3);
    leg->SetMargin(0.5);

    for (int i = 0; i < h.size(); ++i) {
        h[i]->SetLineColor(i == 0 ? kTVibRed : freec+i-1);
        leg->AddEntry(_big(h[i]), (std::to_string(len[i]) + " nm").c_str(), "L");
        h[h.size()-i-1]->Draw("HIST SAME");
    }
    leg->Draw();

    gPad->SetLogy();
    c->SaveAs("../amodel-pdfs-Po210.pdf");
}

void a_plot_U() {

    TFile tfile1("./gerda-pdfs/alphas/pdf-Ra226-pPlus.root", "READ");
    TFile tfile2("./gerda-pdfs/alphas/pdf-Rn222-pPlus.root", "READ");
    TFile tfile3("./gerda-pdfs/alphas/pdf-Po218-pPlus.root", "READ");
    TFile tfile4("./gerda-pdfs/alphas/pdf-Po214-pPlus.root", "READ");
    std::vector<int> len = {0, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    std::vector<TH1D*> h1, h2, h3, h4;
    for (auto& l : len) {
        h1.push_back(dynamic_cast<TH1D*>(tfile1.Get(("hist_dl" + std::to_string(l) + "nm").c_str())));
        h2.push_back(dynamic_cast<TH1D*>(tfile2.Get(("hist_dl" + std::to_string(l) + "nm").c_str())));
        h3.push_back(dynamic_cast<TH1D*>(tfile3.Get(("hist_dl" + std::to_string(l) + "nm").c_str())));
        h4.push_back(dynamic_cast<TH1D*>(tfile4.Get(("hist_dl" + std::to_string(l) + "nm").c_str())));
    }

    // sum
    std::vector<TH1D*> h;
    for (int i = 0; i < h1.size(); ++i) {
        auto _h_tmp = new TH1D(("_h_tmp" + std::to_string(i)).c_str(), "_h_tmp", 8000, 0, 8000);
        for (int j = 1; j <= 7500; ++j) {
            _h_tmp->SetBinContent(j, h1[i]->GetBinContent(j)
                                     + h2[i]->GetBinContent(j)*0.5
                                     + h3[i]->GetBinContent(j)*0.5*0.5
                                     + h4[i]->GetBinContent(j)*0.5*0.5*0.5);
        }
        for (int j = 7501; j <= 8000; ++j) {
            _h_tmp->SetBinContent(j, h3[i]->GetBinContent(j)*0.5*0.5
                                     + h4[i]->GetBinContent(j)*0.5*0.5*0.5);
        }
        h.push_back(_h_tmp);
        h.back()->Scale(1E-9); // normalize to primaries
    }

    // props
    h.back()->GetXaxis()->SetRangeUser(3500, 7999);
    h.back()->GetYaxis()->SetRangeUser(5E-11, 0.5);
    h.back()->SetTitle(";energy [keV];probability / (decay keV)");

    auto c = new TCanvas();
    auto leg = new TLegend(0.1, 0.15, 0.75, 0.4);
    leg->SetHeader("^{226}Ra decay chain on p^{+}", "L");
    leg->SetNColumns(3);
    leg->SetMargin(0.5);

    for (int i = 0; i < h.size(); ++i) {
        h[i]->SetLineColor(i == 0 ? kTVibRed : freec+i-1);
        leg->AddEntry(_big(h[i]), (std::to_string(len[i]) + " nm").c_str(), "L");
        h[h.size()-1-i]->Draw("HIST SAME");
    }
    leg->Draw();

    gPad->SetLogy();
    c->SaveAs("../amodel-pdfs-U.pdf");
}

void g_plot_K40(bool bayes = false) {

    // dummy histogram to extend the range
    auto _h0 = new TH1D("h", "h", 8000, 0, 8000);

    TH1D *h0, *h1, *h2, *h3, *h4, *h5;

    auto set1 = get_pdf_set("minishroud", "ms_all", "K40");
    auto set2 = get_pdf_set("larveto", "fibers", "K40");
    auto set3 = get_pdf_set("larveto", "copper_shroud", "K40");

    if (!bayes) {
        auto& set1_M1 = set1.M1_enrBEGe;
        set1_M1->Add(set1.M1_enrCoax);

        auto& set2_M1 = set2.M1_enrBEGe;
        set2_M1->Add(set2.M1_enrCoax);

        auto& set3_M1 = set3.M1_enrBEGe;
        set3_M1->Add(set3.M1_enrCoax);

        h0 = set1_M1;             h0->Rebin(rebin);
        h1 = set1.M2_enrE1plusE2; h1->Rebin(rebin);
        h2 = set2_M1;             h2->Rebin(rebin);
        h3 = set2.M2_enrE1plusE2; h3->Rebin(rebin);
        h4 = set3_M1;             h4->Rebin(rebin);
        h5 = set3.M2_enrE1plusE2; h5->Rebin(rebin);
    }
    else {
        auto f = new TFile("bb-histos.root");
        h0 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_K40_M1"    ));
        h1 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_K40_M2"    ));
        h2 = dynamic_cast<TH1D*>(f->Get("bb_larveto_fibers_K40_M1"       ));
        h3 = dynamic_cast<TH1D*>(f->Get("bb_larveto_fibers_K40_M2"       ));
        h4 = dynamic_cast<TH1D*>(f->Get("bb_larveto_copper_shroud_K40_M1"));
        h5 = dynamic_cast<TH1D*>(f->Get("bb_larveto_copper_shroud_K40_M2"));
    }

    h0 -> SetLineColorAlpha(kTBriBlue,  1.0);
    h1 -> SetLineColorAlpha(kTBriBlue,  0.5);
    h2 -> SetLineColorAlpha(kTBriRed,   1.0);
    h3 -> SetLineColorAlpha(kTBriRed,   0.5);
    h4 -> SetLineColorAlpha(kTBriGreen, 1.0);
    h5 -> SetLineColorAlpha(kTBriGreen, 0.5);

    TLegend leg(0.14, 0.14, 0.55, 0.34);
    leg.AddEntry(_big(h0), "^{40}K - mini-shroud", "L");
    leg.AddEntry(_big(h2), "^{40}K - fibers", "L");
    leg.AddEntry(_big(h4), "^{40}K - copper shrouds", "L");

    // props
    _h0->GetXaxis()->SetRangeUser(560, 1510);
    _h0->GetYaxis()->SetRangeUser(1.1E-10, 2E-03);
    _h0->GetYaxis()->SetNdivisions(4);
    _h0->SetTitle(";energy [keV];probability / (decay keV)");
    _h0->GetXaxis()->SetNdivisions(8,0,0);
    _h0->GetYaxis()->SetTitleOffset(1.4);

    // draw!
    TCanvas c;
    TPad pad_l("pad_l", "pad_l", 0.0, 0.0, 0.7, 1.0);
    TPad pad_r("pad_r", "pad_r", 0.7, 0.0, 1.0, 1.0);
    pad_l.Draw();
    pad_l.SetLeftMargin(0.13);
    pad_l.SetRightMargin(0.01);
    pad_r.SetLeftMargin(0.02);
    pad_r.Draw();

    pad_l.cd();
    _h0 -> DrawCopy("HIST");
    h0  -> DrawCopy("HIST SAME");
    h1  -> DrawCopy("HIST SAME");
    h2  -> DrawCopy("HIST SAME");
    h3  -> DrawCopy("HIST SAME");
    h4  -> DrawCopy("HIST SAME");
    h5  -> DrawCopy("HIST SAME");
    leg.Draw();

    pad_r.cd();
    _h0 -> GetXaxis()->SetRangeUser(1447, 1473);
    _h0 -> GetXaxis()->SetNdivisions(4,0,0, true);
    _h0 -> GetYaxis()->SetLabelSize(0);
    _h0 -> GetYaxis()->SetTitleSize(0);
    _h0 -> Draw("HIST");
    h0  -> Draw("HIST SAME");
    h1  -> Draw("HIST SAME");
    h2  -> Draw("HIST SAME");
    h3  -> Draw("HIST SAME");
    h4  -> Draw("HIST SAME");
    h5  -> Draw("HIST SAME");

    pad_l.SetLogy();
    pad_r.SetLogy();
    c.SaveAs("../gmodel-pdfs-K40.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
}

void g_plot_K42(bool bayes = false) {

    TH1D *h0, *h1, *h2, *h3, *h4, *h5, *h6, *h7;

    auto set1 = get_pdf_set("lar", "sur_array", "K42");
    auto set2 = get_pdf_set("lar", "above_array", "K42");
    auto set3 = get_pdf_set("gedet", "nplus_bege", "K42");
    auto set4 = get_pdf_set("gedet", "nplus_coax", "K42");

    if (!bayes) {
        auto& set1_M1 = set1.M1_enrBEGe;
        set1_M1->Add(set1.M1_enrCoax);

        auto& set2_M1 = set2.M1_enrBEGe;
        set2_M1->Add(set2.M1_enrCoax);

        auto& set3_M1 = set3.M1_enrBEGe;
        set3_M1->Add(set3.M1_enrCoax);

        auto& set4_M1 = set4.M1_enrBEGe;
        set4_M1->Add(set4.M1_enrCoax);

        h0 = set1_M1;             h0->Rebin(rebin);
        h1 = set1.M2_enrE1plusE2; h1->Rebin(rebin);
        h2 = set2_M1;             h2->Rebin(rebin);
        h3 = set2.M2_enrE1plusE2; h3->Rebin(rebin);
        h4 = set3_M1;             h4->Rebin(rebin);
        h5 = set3.M2_enrE1plusE2; h5->Rebin(rebin);
        h6 = set4_M1;             h6->Rebin(rebin);
        h7 = set4.M2_enrE1plusE2; h7->Rebin(rebin);
    }
    else {
        auto f = new TFile("bb-histos.root");
        h0 = dynamic_cast<TH1D*>(f->Get("bb_lar_sur_array_K42_M1"));
        h1 = dynamic_cast<TH1D*>(f->Get("bb_lar_sur_array_K42_M2"));
        h2 = dynamic_cast<TH1D*>(f->Get("bb_lar_above_array_K42_M1"));
        h3 = dynamic_cast<TH1D*>(f->Get("bb_lar_above_array_K42_M2"));
        h4 = dynamic_cast<TH1D*>(f->Get("bb_gedet_nplus_bege_K42_M1"));
        h5 = dynamic_cast<TH1D*>(f->Get("bb_gedet_nplus_bege_K42_M2"));
        h6 = dynamic_cast<TH1D*>(f->Get("bb_gedet_nplus_coax_K42_M1"));
        h7 = dynamic_cast<TH1D*>(f->Get("bb_gedet_nplus_coax_K42_M2"));
    }

    h0 -> SetLineColorAlpha(kTBriBlue,   1.0);
    h1 -> SetLineColorAlpha(kTBriBlue,   0.5);
    h2 -> SetLineColorAlpha(kTBriRed,    1.0);
    h3 -> SetLineColorAlpha(kTBriRed,    0.5);
    h4 -> SetLineColorAlpha(kTBriGreen,  1.0);
    h5 -> SetLineColorAlpha(kTBriGreen,  0.5);
    h6 -> SetLineColorAlpha(kTBriPurple, 1.0);
    h7 -> SetLineColorAlpha(kTBriPurple, 0.5);

    // props
    h0->SetTitle(";energy [keV];probability / (decay keV)");
    h0->GetXaxis()->SetRangeUser(560, 3499);
    //h0->GetXaxis()->SetRangeUser(1525-30, 1525+30);
    h0->GetYaxis()->SetRangeUser(5E-13, 8E-03);
    // set1_M1->GetYaxis()->SetRangeUser(1E-8, 20);
    h0->GetYaxis()->SetNdivisions(6);

    TLegend leg(0.1, 0.14, 0.5, 0.34);
    leg.AddEntry(_big(h0), "^{42}K - homogeneous", "L");
    leg.AddEntry(_big(h2), "^{42}K - homogeneous above", "L");
    leg.AddEntry(_big(h4), "^{42}K - n^{+} BEGe", "L");
    leg.AddEntry(_big(h6), "^{42}K - n^{+} Coax", "L");

    // draw!
    TCanvas c;
    h0 -> Draw("HIST");
    h1 -> Draw("HIST SAME");
    h2 -> Draw("HIST SAME");
    h3 -> Draw("HIST SAME");
    h4 -> Draw("HIST SAME");
    h5 -> Draw("HIST SAME");
    h6 -> Draw("HIST SAME");
    h7 -> Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../gmodel-pdfs-K42.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
}

void g_plot_Th(bool bayes = false) {

    TH1D *h0, *h1, *h2, *h3;

    auto set1 = get_pdf_set("minishroud", "ms_all", "Bi212");
    auto set2 = get_pdf_set("minishroud", "ms_all", "Tl208");
    auto set3 = get_pdf_set("larveto", "fibers", "Bi212");
    auto set4 = get_pdf_set("larveto", "fibers", "Tl208");

    if (!bayes) {
        auto& set1_M1 = set1.M1_enrBEGe;
        set1_M1->Add(set1.M1_enrCoax);
        set1_M1->Add(set2.M1_enrBEGe);
        set1_M1->Add(set2.M1_enrCoax);
        auto& set1_M2 = set1.M2_enrE1plusE2;
        set1_M2->Add(set2.M2_enrE1plusE2);

        auto& set3_M1 = set3.M1_enrBEGe;
        set3_M1->Add(set3.M1_enrCoax);
        set3_M1->Add(set4.M1_enrBEGe);
        set3_M1->Add(set4.M1_enrCoax);
        auto& set3_M2 = set3.M2_enrE1plusE2;
        set3_M2->Add(set4.M2_enrE1plusE2);

        h0 = set1_M1; h0->Rebin(rebin);
        h1 = set1_M2; h1->Rebin(rebin);
        h2 = set3_M1; h2->Rebin(rebin);
        h3 = set3_M2; h3->Rebin(rebin);
    }
    else {
        auto f = new TFile("bb-histos.root");
        h0 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_Th_M1"));
        h1 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_Th_M2"));
        h2 = dynamic_cast<TH1D*>(f->Get("bb_larveto_fibers_Th_M1"   ));
        h3 = dynamic_cast<TH1D*>(f->Get("bb_larveto_fibers_Th_M2"   ));
    }

    // props
    h0->GetXaxis()->SetRangeUser(560, 3999);
    h0->GetYaxis()->SetRangeUser(3E-11, 5E-03);
    h0->SetTitle(";energy [keV];probability / (decay keV)");

    h0 -> SetLineColorAlpha(kTBriBlue, 1.0);
    h1 -> SetLineColorAlpha(kTBriBlue, 0.5);
    h2 -> SetLineColorAlpha(kTBriRed,  1.0);
    h3 -> SetLineColorAlpha(kTBriRed,  0.5);

    TLegend leg;
    leg.AddEntry(_big(h0), "^{212}Bi + ^{208}Tl - mini-shroud", "L");
    leg.AddEntry(_big(h2), "^{212}Bi + ^{208}Tl - fibers", "L");

    // draw!
    TCanvas c;
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../gmodel-pdfs-Th.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
}

void g_plot_U(bool bayes = false) {

    TH1D *h0, *h1, *h2, *h3;

    auto set1 = get_pdf_set("minishroud", "ms_all", "Pb214");
    auto set2 = get_pdf_set("minishroud", "ms_all", "Bi214");
    auto set3 = get_pdf_set("larveto", "fibers", "Pb214");
    auto set4 = get_pdf_set("larveto", "fibers", "Bi214");

    if (!bayes) {
        auto& set1_M1 = set1.M1_enrBEGe;
        set1_M1->Add(set1.M1_enrCoax);
        set1_M1->Add(set2.M1_enrBEGe);
        set1_M1->Add(set2.M1_enrCoax);
        auto& set1_M2 = set1.M2_enrE1plusE2;
        set1_M2->Add(set2.M2_enrE1plusE2);

        auto& set3_M1 = set3.M1_enrBEGe;
        set3_M1->Add(set3.M1_enrCoax);
        set3_M1->Add(set4.M1_enrBEGe);
        set3_M1->Add(set4.M1_enrCoax);
        auto& set3_M2 = set3.M2_enrE1plusE2;
        set3_M2->Add(set4.M2_enrE1plusE2);

        h0 = set1_M1; h0->Rebin(rebin);
        h1 = set1_M2; h1->Rebin(rebin);
        h2 = set3_M1; h2->Rebin(rebin);
        h3 = set3_M2; h3->Rebin(rebin);
    }
    else {
        auto f = new TFile("bb-histos.root");
        h0 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_U_M1"));
        h1 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_U_M2"));
        h2 = dynamic_cast<TH1D*>(f->Get("bb_larveto_fibers_U_M1"));
        h3 = dynamic_cast<TH1D*>(f->Get("bb_larveto_fibers_U_M2"));
    }

    // props
    h0->GetXaxis()->SetRangeUser(560, 4999);
    h0->GetYaxis()->SetRangeUser(1.1E-10, 5E-03);
    h0->GetYaxis()->SetNdivisions(4);
    h0->SetTitle(";energy [keV];probability / (decay keV)");

    h0 -> SetLineColorAlpha(kTBriBlue, 1.0);
    h1 -> SetLineColorAlpha(kTBriBlue, 0.5);
    h2 -> SetLineColorAlpha(kTBriRed,  1.0);
    h3 -> SetLineColorAlpha(kTBriRed,  0.5);

    TLegend leg;
    leg.AddEntry(_big(h0), "^{214}Pb + ^{214}Bi - mini-shroud", "L");
    leg.AddEntry(_big(h2), "^{214}Pb + ^{214}Bi - fibers", "L");

    // draw!
    TCanvas c;
    h0 -> Draw("HIST");
    h1 -> Draw("HIST SAME");
    h2 -> Draw("HIST SAME");
    h3 -> Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../gmodel-pdfs-U.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
}

void g_plot_misc(bool bayes = false) {

    TH1D *h3, *h4, *h5, *h6;

    auto set4 = get_pdf_set("minishroud", "ms_all", "Pa234m");
    auto set5 = get_pdf_set("minishroud", "ms_all", "Ac228");

    if (!bayes) {
        auto& set4_M1 = set4.M1_enrBEGe;
        set4_M1->Add(set4.M1_enrCoax);

        auto& set5_M1 = set5.M1_enrBEGe;
        set5_M1->Add(set5.M1_enrCoax);

        h3 = set4_M1;             h3->Rebin(rebin);
        h4 = set4.M2_enrE1plusE2; h4->Rebin(rebin);
        h5 = set5_M1;             h5->Rebin(rebin);
        h6 = set5.M2_enrE1plusE2; h6->Rebin(rebin);
    }
    else {
        auto f = new TFile("bb-histos.root");
        h3 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_Pa234m_M1"));
        h4 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_Pa234m_M2"));
        h5 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_Ac228_M1"));
        h6 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_Ac228_M2"));
    }

    // props
    h5->GetXaxis()->SetRangeUser(560, 2200);
    h5->GetYaxis()->SetNdivisions(4);
    h5->SetTitle(";energy [keV];probability / (decay keV)");

    h3 -> SetLineColorAlpha(kTBriGreen,  1.0);
    h4 -> SetLineColorAlpha(kTBriGreen,  0.5);
    h5 -> SetLineColorAlpha(kTBriPurple, 1.0);
    h6 -> SetLineColorAlpha(kTBriPurple, 0.5);

    TLegend leg(0.1, 0.14, 0.45, 0.34);
    leg.AddEntry(_big(h3), "^{234m}Pa - mini-shroud", "L");
    leg.AddEntry(_big(h5), "^{228}Ac - mini-shroud",  "L");

    // draw!
    TCanvas c;
    h5 -> Draw("HIST SAME");
    h6 -> Draw("HIST SAME");
    h3 -> Draw("HIST SAME");
    h4 -> Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../gmodel-pdfs-misc.pdf");

    set4.file->Close();
    set5.file->Close();
}

void g_plot_misc2(bool bayes = false) {

    TH1D *h0, *h1, *h2, *h3, *h4;

    auto set1 = get_pdf_set("cables", "cables_all", "Co60");
    auto set2 = get_pdf_set("gedet", "intrinsic_bege", "2nbb");
    auto set3 = get_pdf_set("gedet", "intrinsic_coax", "2nbb");

    if (!bayes) {
        auto& set1_M1 = set1.M1_enrBEGe;
        set1_M1->Add(set1.M1_enrCoax);

        auto& set2_M1 = set2.M1_enrBEGe;
        set2_M1->Add(set2.M1_enrCoax);
        set2_M1->Add(set3.M1_enrBEGe);
        set2_M1->Add(set3.M1_enrCoax);

        h0 = set1_M1;             h0->Rebin(rebin);
        h1 = set1.M2_enrE1plusE2; h1->Rebin(rebin);
        h2 = set2_M1;             h2->Rebin(rebin);
    }
    else {
        auto f = new TFile("bb-histos.root");
        h0 = dynamic_cast<TH1D*>(f->Get("bb_cables_cables_all_Co60_M1"));
        h1 = dynamic_cast<TH1D*>(f->Get("bb_cables_cables_all_Co60_M2"));
        h2 = dynamic_cast<TH1D*>(f->Get("bb_gedet_intrinsic_2nbb_M1"));
    }

    // props
    h0->GetXaxis()->SetRangeUser(560, 2599);
    h0->GetYaxis()->SetNdivisions(4);
    h0->SetTitle(";energy [keV];probability / (decay keV)");

    h0 -> SetLineColorAlpha(kTBriBlue,   1.0);
    h1 -> SetLineColorAlpha(kTBriBlue,   0.5);
    h2 -> SetLineColorAlpha(kTBriRed,    1.0);

    TLegend leg(0.1, 0.14, 0.45, 0.34);
    leg.AddEntry(_big(h0), "^{60}Co - flat cables",        "L");
    leg.AddEntry(_big(h2), "2#nu#beta#beta",     "L");

    // draw!
    TCanvas c;
    h0 -> Draw("HIST");
    h1 -> Draw("HIST SAME");
    h2 -> Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../gmodel-pdfs-misc2.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
}

void k_plot_K40() {

    TH1D *h0, *h1, *h2, *h3, *h4, *h5, *h6, *h7;

    auto set1 = get_pdf_set("cables", "cables_all", "K40");
    auto set2 = get_pdf_set("ge_holders", "plates", "K40");
    auto set3 = get_pdf_set("minishroud", "ms_all", "K40");
    auto set4 = get_pdf_set("larveto", "fibers", "K40");
    auto set5 = get_pdf_set("larveto", "copper_shroud", "K40");
    auto set6 = get_pdf_set("gedet", "intrinsic_bege", "2nbb");
    auto set7 = get_pdf_set("gedet", "intrinsic_coax", "2nbb");
    auto set8 = get_pdf_set("larveto", "sipm_ring", "K40");
    auto set9 = get_pdf_set("electronics", "cc3", "K40");

    set6.M1_all_1461->Add(set7.M1_all_1461);

    h1 = remove_bin(set1.M1_all_1461);
    h0 = remove_bin(set2.M1_all_1461);
    h2 = remove_bin(set3.M1_all_1461);
    h3 = remove_bin(set4.M1_all_1461);
    h4 = remove_bin(set5.M1_all_1461);
    h5 = remove_bin(set6.M1_all_1461);
    h6 = remove_bin(set8.M1_all_1461);
    h7 = remove_bin(set9.M1_all_1461);

    // props
    h0->GetXaxis()->SetRangeUser(0, 36); // no naturals!
    h0->GetYaxis()->SetRangeUser(2E-08, 1.5E-03);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.003);
    h0->GetYaxis()->SetTitleOffset(1.8);
    h0->GetXaxis()->SetTickSize(0.01);

    h0->SetLineColor(kTBriBlue);
    h1->SetLineColor(kTBriCyan);
    h2->SetLineColor(kTBriPurple);
    h3->SetLineColor(kTBriRed);
    h4->SetLineColor(kTBriGreen);
    h5->SetLineColor(kTMutIndigo);
    h6->SetLineColor(kTMutRose);
    h7->SetLineColor(kTBriCyan);

    for (auto i : {h0, h1, h2, h3, h4, h5, h6, h7}) {
        i->SetFillStyle(1001);
        i->SetLineWidth(lw);
        i->SetFillColorAlpha(i->GetLineColor(), f_alpha);
    }

    TLegend leg(0.35, 0.80, 0.7, 0.98);
    // leg.AddEntry(_big(h1), "^{40}K - cables", "L");
    leg.AddEntry(_big(h0), "^{40}K - holders", "L");
    leg.AddEntry(_big(h2), "^{40}K - mini-shrouds", "L");
    leg.AddEntry(_big(h3), "^{40}K - fiber shroud", "L");
    leg.AddEntry(_big(h4), "^{40}K - copper shrouds", "L");
    leg.AddEntry(_big(h6), "^{40}K - SiPMs", "L");
    leg.AddEntry(_big(h7), "^{40}K - front-end elec.", "L");
    leg.AddEntry(_big(h5), "2#nu#beta#beta", "L");

    // draw!
    TCanvas c("", "", 800, 1000);
    c.GetPad(0)->SetBottomMargin(0.09);
    c.GetPad(0)->SetLeftMargin(0.1);
    h0->Draw("HIST");
    //h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h5->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    h7->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K40.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
    set5.file->Close();
    set6.file->Close();
    set7.file->Close();
    set8.file->Close();
    set9.file->Close();
}

void k_plot_K40_sep() {

    TH1D *h0, *h1, *h2, *h3, *h4, *h5, *h6;

    auto set1 = get_pdf_set("minishroud", "ms1", "K40");
    auto set2 = get_pdf_set("minishroud", "ms2", "K40");
    auto set3 = get_pdf_set("minishroud", "ms3", "K40");
    auto set4 = get_pdf_set("minishroud", "ms4", "K40");
    auto set5 = get_pdf_set("minishroud", "ms5", "K40");
    auto set6 = get_pdf_set("minishroud", "ms6", "K40");
    auto set7 = get_pdf_set("minishroud", "ms7", "K40");

    h0 = remove_bin(set1.M1_all_1461);
    h1 = remove_bin(set2.M1_all_1461);
    h2 = remove_bin(set3.M1_all_1461);
    h3 = remove_bin(set4.M1_all_1461);
    h4 = remove_bin(set5.M1_all_1461);
    h5 = remove_bin(set6.M1_all_1461);
    h6 = remove_bin(set7.M1_all_1461);

    // props
    h0->GetXaxis()->SetRangeUser(0, 36); // no naturals!
    h0->GetYaxis()->SetRangeUser(9E-07, 1.5E-03);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.003);
    h0->GetYaxis()->SetTitleOffset(1.8);
    h0->GetXaxis()->SetTickSize(0.01);

    h1->SetLineColor(kTMutIndigo);
    h2->SetLineColor(kTMutCyan  );
    h3->SetLineColor(kTMutTeal  );
    h4->SetLineColor(kTMutGreen );
    h5->SetLineColor(kTMutOlive );
    h6->SetLineColor(kTMutPurple);
    h0->SetLineColor(kTMutRose  );

    h0->SetFillColorAlpha(h0->GetLineColor(), f_alpha);
    h1->SetFillColorAlpha(h1->GetLineColor(), f_alpha);
    h2->SetFillColorAlpha(h2->GetLineColor(), f_alpha);
    h3->SetFillColorAlpha(h3->GetLineColor(), f_alpha);
    h4->SetFillColorAlpha(h4->GetLineColor(), f_alpha);
    h5->SetFillColorAlpha(h5->GetLineColor(), f_alpha);
    h6->SetFillColorAlpha(h6->GetLineColor(), f_alpha);

    for (auto i : {h0, h1, h2, h3, h4, h5, h6}) {
        i->SetFillStyle(1001);
        i->SetLineWidth(lw);
    }

    TLegend leg(0.37, 0.8, 0.72, 0.98);
    leg.AddEntry(_big(h1), "^{40}K - close to S1", "L");
    leg.AddEntry(_big(h2), "^{40}K - close to S2", "L");
    leg.AddEntry(_big(h3), "^{40}K - close to S3", "L");
    leg.AddEntry(_big(h4), "^{40}K - close to S4", "L");
    leg.AddEntry(_big(h5), "^{40}K - close to S5", "L");
    leg.AddEntry(_big(h6), "^{40}K - close to S6", "L");
    leg.AddEntry(_big(h0), "^{40}K - close to S7", "L");

    // draw!
    TCanvas c("", "", 800, 1000);
    c.GetPad(0)->SetBottomMargin(0.09);
    c.GetPad(0)->SetLeftMargin(0.1);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h5->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K40-sep.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
    set5.file->Close();
    set6.file->Close();
    set7.file->Close();
}

void k_plot_K42() {

    TH1D *h0, *h1, *h2, *h3, *h4;

    auto set1 = get_pdf_set("lar", "sur_array", "K42");
    auto set2 = get_pdf_set("lar", "above_array", "K42");
    auto set3 = get_pdf_set("gedet", "nplus_bege", "K42");
    auto set4 = get_pdf_set("gedet", "nplus_coax", "K42");
    auto set5 = get_pdf_set("gedet", "intrinsic_bege", "2nbb");
    auto set6 = get_pdf_set("gedet", "intrinsic_coax", "2nbb");

    set5.M1_all_1525->Add(set6.M1_all_1525);

    h0 = remove_bin(set1.M1_all_1525);
    h1 = remove_bin(set2.M1_all_1525);
    h2 = remove_bin(set3.M1_all_1525);
    h3 = remove_bin(set4.M1_all_1525);
    h4 = remove_bin(set5.M1_all_1525);

    // props
    h0->GetXaxis()->SetRangeUser(0, 36); // no naturals!
    h0->GetYaxis()->SetRangeUser(2E-09, 5E-03);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.003);
    h0->GetYaxis()->SetTitleOffset(1.8);
    h0->GetXaxis()->SetTickSize(0.01);

    h0->SetLineColor(kTBriBlue);
    h1->SetLineColor(kTBriCyan);
    h2->SetLineColor(kTBriGreen);
    h3->SetLineColor(kTBriRed);
    h4->SetLineColor(kTBriPurple);

    h0->SetFillColorAlpha(h0->GetLineColor(), f_alpha);
    h1->SetFillColorAlpha(h1->GetLineColor(), f_alpha);
    h2->SetFillColorAlpha(h2->GetLineColor(), f_alpha);
    h3->SetFillColorAlpha(h3->GetLineColor(), f_alpha);
    h4->SetFillColorAlpha(h4->GetLineColor(), f_alpha);

    for (auto i : {h0, h1, h2, h3, h4}) {
        i->SetFillStyle(1001);
        i->SetLineWidth(lw);
    }

    TLegend leg(0.35, 0.85, 0.7, 0.97);
    // leg.AddEntry(_big(h1), "^{40}K - cables", "L");
    leg.AddEntry(_big(h0), "^{42}K - LAr hom.", "L");
    leg.AddEntry(_big(h1), "^{42}K - LAr above A.", "L");
    leg.AddEntry(_big(h2), "^{42}K - n^{+} BEGe", "L");
    leg.AddEntry(_big(h3), "^{42}K - n^{+} Coax", "L");
    leg.AddEntry(_big(h4), "2#nu#beta#beta", "L");

    // draw!
    TCanvas c("", "", 800, 1000);
    c.GetPad(0)->SetBottomMargin(0.09);
    c.GetPad(0)->SetLeftMargin(0.1);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K42.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
    set5.file->Close();
    set6.file->Close();
}

void k_plot_K42_sep() {

    TH1D *h0, *h1, *h2, *h3, *h4, *h5, *h6;

    auto set1 = get_pdf_set("lar", "above_ms1", "K42");
    auto set2 = get_pdf_set("lar", "above_ms2", "K42");
    auto set3 = get_pdf_set("lar", "above_ms3", "K42");
    auto set4 = get_pdf_set("lar", "above_ms4", "K42");
    auto set5 = get_pdf_set("lar", "above_ms5", "K42");
    auto set6 = get_pdf_set("lar", "above_ms6", "K42");
    auto set7 = get_pdf_set("lar", "above_ms7", "K42");

    h0 = remove_bin(set1.M1_all_1525);
    h1 = remove_bin(set2.M1_all_1525);
    h2 = remove_bin(set3.M1_all_1525);
    h3 = remove_bin(set4.M1_all_1525);
    h4 = remove_bin(set5.M1_all_1525);
    h5 = remove_bin(set6.M1_all_1525);
    h6 = remove_bin(set7.M1_all_1525);

    // props
    h0->GetXaxis()->SetRangeUser(0, 36); // no naturals!
    h0->GetYaxis()->SetRangeUser(9E-09, 3E-02);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.003);
    h0->GetYaxis()->SetTitleOffset(1.8);
    h0->GetXaxis()->SetTickSize(0.01);

    h0->SetLineColor(kTMutIndigo);
    h1->SetLineColor(kTMutCyan  );
    h2->SetLineColor(kTMutTeal  );
    h3->SetLineColor(kTMutGreen );
    h4->SetLineColor(kTMutOlive );
    h5->SetLineColor(kTMutPurple);
    h6->SetLineColor(kTMutRose  );

    h0->SetFillColorAlpha(h0->GetLineColor(), f_alpha);
    h1->SetFillColorAlpha(h1->GetLineColor(), f_alpha);
    h2->SetFillColorAlpha(h2->GetLineColor(), f_alpha);
    h3->SetFillColorAlpha(h3->GetLineColor(), f_alpha);
    h4->SetFillColorAlpha(h4->GetLineColor(), f_alpha);
    h5->SetFillColorAlpha(h5->GetLineColor(), f_alpha);
    h6->SetFillColorAlpha(h6->GetLineColor(), f_alpha);

    for (auto i : {h0, h1, h2, h3, h4, h5, h6}) {
        i->SetFillStyle(1001);
        i->SetLineWidth(lw);
    }

    TLegend leg(0.11, 0.85, 0.90, 0.98);
    leg.SetNColumns(2);
    leg.AddEntry(_big(h0), "^{42}K - above S1", "L");
    leg.AddEntry(_big(h1), "^{42}K - above S2", "L");
    leg.AddEntry(_big(h2), "^{42}K - above S3", "L");
    leg.AddEntry(_big(h3), "^{42}K - above S4", "L");
    leg.AddEntry(_big(h4), "^{42}K - above S5", "L");
    leg.AddEntry(_big(h5), "^{42}K - above S6", "L");
    leg.AddEntry(_big(h6), "^{42}K - above S7", "L");

    // draw!
    TCanvas c("", "", 800, 1000);
    c.GetPad(0)->SetBottomMargin(0.09);
    c.GetPad(0)->SetLeftMargin(0.1);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h5->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K42-sep.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
    set5.file->Close();
    set6.file->Close();
    set7.file->Close();
}

void k_plot_K40_M2() {

    TH1D *h0, *h1, *h2, *h3, *h4, *h5, *h6, *h7;

    auto set1 = get_pdf_set("cables",      "cables_all",    "K40");
    auto set2 = get_pdf_set("ge_holders",  "plates",        "K40");
    auto set3 = get_pdf_set("minishroud",  "ms_all",        "K40");
    auto set4 = get_pdf_set("larveto",     "fibers",        "K40");
    auto set5 = get_pdf_set("larveto",     "copper_shroud", "K40");
    auto set8 = get_pdf_set("larveto",     "sipm_ring",     "K40");
    auto set9 = get_pdf_set("electronics", "cc3",           "K40");

    h1 = set1.M2_ID1vsID2_1461->ProjectionX("_1"); h1->Add(set1.M2_ID1vsID2_1461->ProjectionY());
    h0 = set2.M2_ID1vsID2_1461->ProjectionX("_0"); h0->Add(set2.M2_ID1vsID2_1461->ProjectionY());
    h2 = set3.M2_ID1vsID2_1461->ProjectionX("_2"); h2->Add(set3.M2_ID1vsID2_1461->ProjectionY());
    h3 = set4.M2_ID1vsID2_1461->ProjectionX("_3"); h3->Add(set4.M2_ID1vsID2_1461->ProjectionY());
    h4 = set5.M2_ID1vsID2_1461->ProjectionX("_4"); h4->Add(set5.M2_ID1vsID2_1461->ProjectionY());
    h6 = set8.M2_ID1vsID2_1461->ProjectionX("_6"); h6->Add(set8.M2_ID1vsID2_1461->ProjectionY());
    h7 = set9.M2_ID1vsID2_1461->ProjectionX("_7"); h7->Add(set9.M2_ID1vsID2_1461->ProjectionY());

    h1 = remove_bin(h1);
    h0 = remove_bin(h0);
    h2 = remove_bin(h2);
    h3 = remove_bin(h3);
    h4 = remove_bin(h4);
    h6 = remove_bin(h6);
    h7 = remove_bin(h7);

    // props
    h0->GetXaxis()->SetRangeUser(0, 36); // no naturals!
    h0->GetYaxis()->SetRangeUser(2E-10, 2E-03);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.003);
    h0->GetYaxis()->SetTitleOffset(1.8);
    h0->GetXaxis()->SetTickSize(0.01);

    h0->SetLineColor(kTBriBlue);
    h1->SetLineColor(kTBriCyan);
    h2->SetLineColor(kTBriPurple);
    h3->SetLineColor(kTBriRed);
    h4->SetLineColor(kTBriGreen);
    h6->SetLineColor(kTMutRose);
    h7->SetLineColor(kTBriCyan);

    for (auto i : {h0, h1, h2, h3, h4, h6, h7}) {
        i->SetFillStyle(1001);
        i->SetLineWidth(lw);
        i->SetFillColorAlpha(i->GetLineColor(), f_alpha);
    }

    TLegend leg(0.11, 0.82, 0.51, 0.98);
    // leg.AddEntry(_big(h1), "^{40}K - cables", "L");
    leg.AddEntry(_big(h0), "^{40}K - holders", "L");
    leg.AddEntry(_big(h2), "^{40}K - mini-shrouds", "L");
    leg.AddEntry(_big(h3), "^{40}K - fiber shroud", "L");
    leg.AddEntry(_big(h4), "^{40}K - copper shrouds", "L");
    leg.AddEntry(_big(h6), "^{40}K - SiPMs", "L");
    leg.AddEntry(_big(h7), "^{40}K - front-end elec.", "L");

    // draw!
    TCanvas c("", "", 800, 1000);
    c.GetPad(0)->SetBottomMargin(0.09);
    c.GetPad(0)->SetLeftMargin(0.1);
    h0->Draw("HIST");
    //h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    h7->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K40-M2.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
    set5.file->Close();
    set8.file->Close();
    set9.file->Close();
}

void k_plot_K42_M2() {

    TH1D *h0, *h1, *h2, *h3, *h4;

    auto set1 = get_pdf_set("lar", "sur_array", "K42");
    auto set2 = get_pdf_set("lar", "above_array", "K42");
    auto set3 = get_pdf_set("gedet", "nplus_bege", "K42");
    auto set4 = get_pdf_set("gedet", "nplus_coax", "K42");

    h0 = set1.M2_ID1vsID2_1525->ProjectionX("_0"); h0->Add(set1.M2_ID1vsID2_1525->ProjectionY());
    h1 = set2.M2_ID1vsID2_1525->ProjectionX("_1"); h1->Add(set2.M2_ID1vsID2_1525->ProjectionY());
    h2 = set3.M2_ID1vsID2_1525->ProjectionX("_2"); h2->Add(set3.M2_ID1vsID2_1525->ProjectionY());
    h3 = set4.M2_ID1vsID2_1525->ProjectionX("_3"); h3->Add(set4.M2_ID1vsID2_1525->ProjectionY());

    h0 = remove_bin(h0);
    h1 = remove_bin(h1);
    h2 = remove_bin(h2);
    h3 = remove_bin(h3);

    // props
    h0->GetXaxis()->SetRangeUser(0, 36); // no naturals!
    h0->GetYaxis()->SetRangeUser(2E-09, 2E-03);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.003);
    h0->GetYaxis()->SetTitleOffset(1.8);
    h0->GetXaxis()->SetTickSize(0.01);

    h0->SetLineColor(kTBriBlue);
    h1->SetLineColor(kTBriCyan);
    h2->SetLineColor(kTBriGreen);
    h3->SetLineColor(kTBriRed);

    h0->SetFillColorAlpha(h0->GetLineColor(), f_alpha);
    h1->SetFillColorAlpha(h1->GetLineColor(), f_alpha);
    h2->SetFillColorAlpha(h2->GetLineColor(), f_alpha);
    h3->SetFillColorAlpha(h3->GetLineColor(), f_alpha);

    for (auto i : {h0, h1, h2, h3}) {
        i->SetFillStyle(1001);
        i->SetLineWidth(lw);
    }

    TLegend leg(0.11, 0.85, 0.51, 0.97);
    // leg.AddEntry(_big(h1), "^{40}K - cables", "L");
    leg.AddEntry(_big(h0), "^{42}K - LAr hom.", "L");
    leg.AddEntry(_big(h1), "^{42}K - LAr above A.", "L");
    leg.AddEntry(_big(h2), "^{42}K - n^{+} BEGe", "L");
    leg.AddEntry(_big(h3), "^{42}K - n^{+} Coax", "L");

    // draw!
    TCanvas c("", "", 800, 1000);
    c.GetPad(0)->SetBottomMargin(0.09);
    c.GetPad(0)->SetLeftMargin(0.1);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K42-M2.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
}

void k_plot_K40_sep_M2() {

    TH1D *h0, *h1, *h2, *h3, *h4, *h5, *h6;

    auto set1 = get_pdf_set("minishroud", "ms1", "K40");
    auto set2 = get_pdf_set("minishroud", "ms2", "K40");
    auto set3 = get_pdf_set("minishroud", "ms3", "K40");
    auto set4 = get_pdf_set("minishroud", "ms4", "K40");
    auto set5 = get_pdf_set("minishroud", "ms5", "K40");
    auto set6 = get_pdf_set("minishroud", "ms6", "K40");
    auto set7 = get_pdf_set("minishroud", "ms7", "K40");

    h0 = set1.M2_ID1vsID2_1461->ProjectionX("_0"); h0->Add(set1.M2_ID1vsID2_1461->ProjectionY());
    h1 = set2.M2_ID1vsID2_1461->ProjectionX("_1"); h1->Add(set2.M2_ID1vsID2_1461->ProjectionY());
    h2 = set3.M2_ID1vsID2_1461->ProjectionX("_2"); h2->Add(set3.M2_ID1vsID2_1461->ProjectionY());
    h3 = set4.M2_ID1vsID2_1461->ProjectionX("_3"); h3->Add(set4.M2_ID1vsID2_1461->ProjectionY());
    h4 = set5.M2_ID1vsID2_1461->ProjectionX("_4"); h4->Add(set5.M2_ID1vsID2_1461->ProjectionY());
    h5 = set6.M2_ID1vsID2_1461->ProjectionX("_5"); h5->Add(set6.M2_ID1vsID2_1461->ProjectionY());
    h6 = set7.M2_ID1vsID2_1461->ProjectionX("_6"); h6->Add(set7.M2_ID1vsID2_1461->ProjectionY());

    h0 = remove_bin(h0);
    h1 = remove_bin(h1);
    h2 = remove_bin(h2);
    h3 = remove_bin(h3);
    h4 = remove_bin(h4);
    h5 = remove_bin(h5);
    h6 = remove_bin(h6);

    // props
    h0->GetXaxis()->SetRangeUser(0, 36); // no naturals!
    h0->GetYaxis()->SetRangeUser(1.5E-07, 7E-04);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.003);
    h0->GetYaxis()->SetTitleOffset(1.8);
    h0->GetXaxis()->SetTickSize(0.01);

    h1->SetLineColor(kTMutIndigo);
    h2->SetLineColor(kTMutCyan  );
    h3->SetLineColor(kTMutTeal  );
    h4->SetLineColor(kTMutGreen );
    h5->SetLineColor(kTMutOlive );
    h6->SetLineColor(kTMutPurple);
    h0->SetLineColor(kTMutRose  );

    h0->SetFillColorAlpha(h0->GetLineColor(), f_alpha);
    h1->SetFillColorAlpha(h1->GetLineColor(), f_alpha);
    h2->SetFillColorAlpha(h2->GetLineColor(), f_alpha);
    h3->SetFillColorAlpha(h3->GetLineColor(), f_alpha);
    h4->SetFillColorAlpha(h4->GetLineColor(), f_alpha);
    h5->SetFillColorAlpha(h5->GetLineColor(), f_alpha);
    h6->SetFillColorAlpha(h6->GetLineColor(), f_alpha);

    for (auto i : {h0, h1, h2, h3, h4, h5, h6}) {
        i->SetFillStyle(1001);
        i->SetLineWidth(lw);
    }

    TLegend leg(0.11, 0.85, 0.90, 0.98);
    leg.SetNColumns(2);
    leg.AddEntry(_big(h1), "^{40}K - close to S1", "L");
    leg.AddEntry(_big(h2), "^{40}K - close to S2", "L");
    leg.AddEntry(_big(h3), "^{40}K - close to S3", "L");
    leg.AddEntry(_big(h4), "^{40}K - close to S4", "L");
    leg.AddEntry(_big(h5), "^{40}K - close to S5", "L");
    leg.AddEntry(_big(h6), "^{40}K - close to S6", "L");
    leg.AddEntry(_big(h0), "^{40}K - close to S7", "L");

    // draw!
    TCanvas c("", "", 800, 1000);
    c.GetPad(0)->SetBottomMargin(0.09);
    c.GetPad(0)->SetLeftMargin(0.1);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h5->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K40-sep-M2.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
    set5.file->Close();
    set6.file->Close();
    set7.file->Close();
}

void k_plot_K42_sep_M2() {

    TH1D *h0, *h1, *h2, *h3, *h4, *h5, *h6;

    auto set1 = get_pdf_set("lar", "above_ms1", "K42");
    auto set2 = get_pdf_set("lar", "above_ms2", "K42");
    auto set3 = get_pdf_set("lar", "above_ms3", "K42");
    auto set4 = get_pdf_set("lar", "above_ms4", "K42");
    auto set5 = get_pdf_set("lar", "above_ms5", "K42");
    auto set6 = get_pdf_set("lar", "above_ms6", "K42");
    auto set7 = get_pdf_set("lar", "above_ms7", "K42");

    h0 = set1.M2_ID1vsID2_1525->ProjectionX("_0"); h0->Add(set1.M2_ID1vsID2_1525->ProjectionY());
    h1 = set2.M2_ID1vsID2_1525->ProjectionX("_1"); h1->Add(set2.M2_ID1vsID2_1525->ProjectionY());
    h2 = set3.M2_ID1vsID2_1525->ProjectionX("_2"); h2->Add(set3.M2_ID1vsID2_1525->ProjectionY());
    h3 = set4.M2_ID1vsID2_1525->ProjectionX("_3"); h3->Add(set4.M2_ID1vsID2_1525->ProjectionY());
    h4 = set5.M2_ID1vsID2_1525->ProjectionX("_4"); h4->Add(set5.M2_ID1vsID2_1525->ProjectionY());
    h5 = set6.M2_ID1vsID2_1525->ProjectionX("_5"); h5->Add(set6.M2_ID1vsID2_1525->ProjectionY());
    h6 = set7.M2_ID1vsID2_1525->ProjectionX("_6"); h6->Add(set7.M2_ID1vsID2_1525->ProjectionY());

    h0 = remove_bin(h0);
    h1 = remove_bin(h1);
    h2 = remove_bin(h2);
    h3 = remove_bin(h3);
    h4 = remove_bin(h4);
    h5 = remove_bin(h5);
    h6 = remove_bin(h6);

    // props
    h0->GetXaxis()->SetRangeUser(0, 36); // no naturals!
    h0->GetYaxis()->SetRangeUser(9E-09, 5E-03);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.003);
    h0->GetYaxis()->SetTitleOffset(1.8);
    h0->GetXaxis()->SetTickSize(0.01);

    h0->SetLineColor(kTMutIndigo);
    h1->SetLineColor(kTMutCyan  );
    h2->SetLineColor(kTMutTeal  );
    h3->SetLineColor(kTMutGreen );
    h4->SetLineColor(kTMutOlive );
    h5->SetLineColor(kTMutPurple);
    h6->SetLineColor(kTMutRose  );

    h0->SetFillColorAlpha(h0->GetLineColor(), f_alpha);
    h1->SetFillColorAlpha(h1->GetLineColor(), f_alpha);
    h2->SetFillColorAlpha(h2->GetLineColor(), f_alpha);
    h3->SetFillColorAlpha(h3->GetLineColor(), f_alpha);
    h4->SetFillColorAlpha(h4->GetLineColor(), f_alpha);
    h5->SetFillColorAlpha(h5->GetLineColor(), f_alpha);
    h6->SetFillColorAlpha(h6->GetLineColor(), f_alpha);

    for (auto i : {h0, h1, h2, h3, h4, h5, h6}) {
        i->SetFillStyle(1001);
        i->SetLineWidth(lw);
    }

    TLegend leg(0.11, 0.85, 0.90, 0.98);
    leg.SetNColumns(2);
    leg.AddEntry(_big(h0), "^{42}K - above S1", "L");
    leg.AddEntry(_big(h1), "^{42}K - above S2", "L");
    leg.AddEntry(_big(h2), "^{42}K - above S3", "L");
    leg.AddEntry(_big(h3), "^{42}K - above S4", "L");
    leg.AddEntry(_big(h4), "^{42}K - above S5", "L");
    leg.AddEntry(_big(h5), "^{42}K - above S6", "L");
    leg.AddEntry(_big(h6), "^{42}K - above S7", "L");

    // draw!
    TCanvas c("", "", 800, 1000);
    c.GetPad(0)->SetBottomMargin(0.09);
    c.GetPad(0)->SetLeftMargin(0.1);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h5->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K42-sep-M2.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
    set5.file->Close();
    set6.file->Close();
    set7.file->Close();
}

void pdfs_plot() {

    tolcols::init();

    gROOT->Macro("gerda-style-short.C");
    gStyle->SetLineScalePS(2.2);

    gROOT->ForceStyle();
    gROOT->SetBatch();

    if (use_bb) {
        compute_bayes_blocks();
    }

    float f = 10;
    for (int i = 0; i < 11; ++i) new TColor(freec+i, (0+i*f)/255., (51+i*f)/255., (102+i*f)/255., "blue");

    // a_plot_Po210();
    // a_plot_U();

    // k_plot_K40();
    // k_plot_K42();
    // k_plot_K40_sep();
    // k_plot_K42_sep();

    // k_plot_K40_M2();
    // k_plot_K42_M2();
    // k_plot_K40_sep_M2();
    k_plot_K42_sep_M2();

    // g_plot_K40(use_bb);
    // g_plot_K42(use_bb);
    // g_plot_Th(use_bb);
    // g_plot_U(use_bb);
    // g_plot_misc(use_bb);
    // g_plot_misc2(use_bb);

    std::cout << "All work completed." << std::endl;
}

TH1D* _big(TH1D* h) {
    auto H = dynamic_cast<TH1D*>(h->Clone());
    H->SetLineWidth(3);
    return H;
}

TH1D* remove_bin(TH1D* h) {
    auto h_r = new TH1D(h->GetName(), h->GetTitle(), h->GetNbinsX()-1, 0, h->GetNbinsX()-1);
    for (int i = 1; i <= h->GetNbinsX()-1; ++i) {
        if (i < 7) h_r->SetBinContent(i, h->GetBinContent(i));
        else       h_r->SetBinContent(i, h->GetBinContent(i+1));
    }
    return h_r;
};

void compute_bayes_blocks() {

    // K42
    auto set1 = get_pdf_set("lar", "sur_array", "K42");
    auto& set1_M1 = set1.M1_enrBEGe;
    set1_M1->Add(set1.M1_enrCoax);

    auto set2 = get_pdf_set("lar", "above_array", "K42");
    auto& set2_M1 = set2.M1_enrBEGe;
    set2_M1->Add(set2.M1_enrCoax);

    auto set3 = get_pdf_set("gedet", "nplus_bege", "K42");
    auto& set3_M1 = set3.M1_enrBEGe;
    set3_M1->Add(set3.M1_enrCoax);

    auto set4 = get_pdf_set("gedet", "nplus_coax", "K42");
    auto& set4_M1 = set4.M1_enrBEGe;
    set4_M1->Add(set4.M1_enrCoax);

    compute_and_save(set1_M1            , set1.nedep, "bb_lar_sur_array_K42_M1"       );
    compute_and_save(set1.M2_enrE1plusE2, set1.ncoin, "bb_lar_sur_array_K42_M2"       );
    set1.file->Close();
    compute_and_save(set2_M1            , set2.nedep, "bb_lar_above_array_K42_M1"     );
    compute_and_save(set2.M2_enrE1plusE2, set2.ncoin, "bb_lar_above_array_K42_M2"     );
    set2.file->Close();
    compute_and_save(set3_M1            , set3.nedep, "bb_gedet_nplus_bege_K42_M1"    );
    compute_and_save(set3.M2_enrE1plusE2, set3.ncoin, "bb_gedet_nplus_bege_K42_M2"    );
    set3.file->Close();
    compute_and_save(set4_M1            , set4.nedep, "bb_gedet_nplus_coax_K42_M1"    );
    compute_and_save(set4.M2_enrE1plusE2, set4.ncoin, "bb_gedet_nplus_coax_K42_M2"    );
    set4.file->Close();

    // misc
    auto set5 = get_pdf_set("cables", "cables_all", "Co60");
    auto& set5_M1 = set5.M1_enrBEGe;
    set5_M1->Add(set5.M1_enrCoax);

    auto set6 = get_pdf_set("gedet", "intrinsic_bege", "2nbb");
    auto set7 = get_pdf_set("gedet", "intrinsic_coax", "2nbb");
    auto& set6_M1 = set6.M1_enrBEGe;
    set6_M1->Add(set6.M1_enrCoax);
    set6_M1->Add(set7.M1_enrBEGe);
    set6_M1->Add(set7.M1_enrCoax);

    auto set8 = get_pdf_set("minishroud", "ms_all", "Pa234m");
    auto& set8_M1 = set8.M1_enrBEGe;
    set8_M1->Add(set8.M1_enrCoax);

    auto set9 = get_pdf_set("minishroud", "ms_all", "Ac228");
    auto& set9_M1 = set9.M1_enrBEGe;
    set9_M1->Add(set9.M1_enrCoax);

    compute_and_save(set5_M1            , set5.nedep, "bb_cables_cables_all_Co60_M1"  );
    compute_and_save(set5.M2_enrE1plusE2, set5.ncoin, "bb_cables_cables_all_Co60_M2"  );
    set5.file->Close();
    compute_and_save(set6_M1            , set6.nedep + set7.nedep, "bb_gedet_intrinsic_2nbb_M1");
    set6.file->Close();
    set7.file->Close();
    compute_and_save(set8_M1            , set8.nedep, "bb_minishroud_ms_all_Pa234m_M1");
    compute_and_save(set8.M2_enrE1plusE2, set8.ncoin, "bb_minishroud_ms_all_Pa234m_M2");
    set8.file->Close();
    compute_and_save(set9_M1            , set9.nedep, "bb_minishroud_ms_all_Ac228_M1" );
    compute_and_save(set9.M2_enrE1plusE2, set9.ncoin, "bb_minishroud_ms_all_Ac228_M2" );
    set9.file->Close();

    // K40
    auto set10 = get_pdf_set("minishroud", "ms_all", "K40");
    auto& set10_M1 = set10.M1_enrBEGe;
    set10_M1->Add(set10.M1_enrCoax);

    auto set11 = get_pdf_set("larveto", "fibers", "K40");
    auto& set11_M1 = set11.M1_enrBEGe;
    set11_M1->Add(set11.M1_enrCoax);

    auto set12 = get_pdf_set("larveto", "copper_shroud", "K40");
    auto& set12_M1 = set12.M1_enrBEGe;
    set12_M1->Add(set12.M1_enrCoax);

    compute_and_save(set10_M1            , set10.nedep, "bb_minishroud_ms_all_K40_M1"    );
    compute_and_save(set10.M2_enrE1plusE2, set10.ncoin, "bb_minishroud_ms_all_K40_M2"    );
    set10.file->Close();
    compute_and_save(set11_M1            , set11.nedep, "bb_larveto_fibers_K40_M1"       );
    compute_and_save(set11.M2_enrE1plusE2, set11.ncoin, "bb_larveto_fibers_K40_M2"       );
    set11.file->Close();
    compute_and_save(set12_M1            , set12.nedep, "bb_larveto_copper_shroud_K40_M1");
    compute_and_save(set12.M2_enrE1plusE2, set12.ncoin, "bb_larveto_copper_shroud_K40_M2");
    set12.file->Close();

    // Th
    auto set13 = get_pdf_set("minishroud", "ms_all", "Bi212", false);
    auto set14 = get_pdf_set("minishroud", "ms_all", "Tl208", false);
    auto& set13_M1 = set13.M1_enrBEGe;
    set13_M1->Add(set13.M1_enrCoax);
    set13_M1->Add(set14.M1_enrBEGe);
    set13_M1->Add(set14.M1_enrCoax);
    auto& set13_M2 = set13.M2_enrE1plusE2;
    set13_M2->Add(set14.M2_enrE1plusE2);

    auto set15 = get_pdf_set("larveto", "fibers", "Bi212", false);
    auto set16 = get_pdf_set("larveto", "fibers", "Tl208", false);
    auto& set15_M1 = set15.M1_enrBEGe;
    set15_M1->Add(set15.M1_enrCoax);
    set15_M1->Add(set16.M1_enrBEGe);
    set15_M1->Add(set16.M1_enrCoax);
    auto& set15_M2 = set15.M2_enrE1plusE2;
    set15_M2->Add(set16.M2_enrE1plusE2);

    set13_M1             -> Scale(1./(set13.nedep + set14.nedep));
    set13.M2_enrE1plusE2 -> Scale(1./(set13.ncoin + set14.ncoin));
    set15_M1             -> Scale(1./(set15.nedep + set16.nedep));
    set15.M2_enrE1plusE2 -> Scale(1./(set15.ncoin + set16.ncoin));

    compute_and_save(set13_M1            , set13.nedep + set14.nedep, "bb_minishroud_ms_all_Th_M1");
    compute_and_save(set13.M2_enrE1plusE2, set13.ncoin + set14.ncoin, "bb_minishroud_ms_all_Th_M2");
    set13.file->Close();
    set14.file->Close();
    compute_and_save(set15_M1            , set15.nedep + set16.nedep, "bb_larveto_fibers_Th_M1");
    compute_and_save(set15.M2_enrE1plusE2, set15.ncoin + set16.ncoin, "bb_larveto_fibers_Th_M2");
    set15.file->Close();
    set16.file->Close();

    // U
    auto set17 = get_pdf_set("minishroud", "ms_all", "Pb214", false);
    auto set18 = get_pdf_set("minishroud", "ms_all", "Bi214", false);
    auto& set17_M1 = set17.M1_enrBEGe;
    set17_M1->Add(set17.M1_enrCoax);
    set17_M1->Add(set18.M1_enrBEGe);
    set17_M1->Add(set18.M1_enrCoax);
    auto& set17_M2 = set17.M2_enrE1plusE2;
    set17_M2->Add(set18.M2_enrE1plusE2);

    auto set19 = get_pdf_set("larveto", "fibers", "Pb214", false);
    auto set20 = get_pdf_set("larveto", "fibers", "Bi214", false);
    auto& set19_M1 = set19.M1_enrBEGe;
    set19_M1->Add(set19.M1_enrCoax);
    set19_M1->Add(set20.M1_enrBEGe);
    set19_M1->Add(set20.M1_enrCoax);
    auto& set19_M2 = set19.M2_enrE1plusE2;
    set19_M2->Add(set20.M2_enrE1plusE2);

    set17_M1             -> Scale(1./(set17.nedep + set18.nedep));
    set17.M2_enrE1plusE2 -> Scale(1./(set17.ncoin + set18.ncoin));
    set19_M1             -> Scale(1./(set19.nedep + set20.nedep));
    set19.M2_enrE1plusE2 -> Scale(1./(set19.ncoin + set20.ncoin));

    compute_and_save(set17_M1            , set17.nedep + set18.nedep, "bb_minishroud_ms_all_U_M1");
    compute_and_save(set17.M2_enrE1plusE2, set17.ncoin + set18.ncoin, "bb_minishroud_ms_all_U_M2");
    set17.file->Close();
    set18.file->Close();
    compute_and_save(set19_M1            , set19.nedep + set20.nedep, "bb_larveto_fibers_U_M1");
    compute_and_save(set19.M2_enrE1plusE2, set19.ncoin + set20.ncoin, "bb_larveto_fibers_U_M2");
    set19.file->Close();
    set20.file->Close();
}

void compute_and_save(TH1D* h, double scale, std::string name, bool force) {
    h->Scale(scale);
    TFile fout("bb-histos.root", "UPDATE");
    auto h_tmp = dynamic_cast<TH1D*>(fout.Get(name.c_str()));
    if (!h_tmp or force) {
        std::cout << "Creating " << name << "...\n";
        auto hr = dynamic_cast<TH1D*>(BayesianBlocks::rebin(h, 0.01, true));
        hr->Scale(1./scale);
        hr->Write(name.c_str(), TObject::kOverwrite);
    }
}

// vim: foldmethod=syntax

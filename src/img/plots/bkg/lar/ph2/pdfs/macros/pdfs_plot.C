/* pdfs_plot.C
 *
 * Author: Luigi Pertoldi - pertoldi@pd.infn.it
 * Created: Tue 1 Jan 2019
 *
 */

#include "../../../../../../tools/bayesian_blocks.hpp"
#include "../../../../../../tools/pdf_handling.hpp"
#include "../../../../../../tools/paul_tol_colors.hpp"
using namespace tolcols;

// color alpha
double f_alpha = 0.05;

// line width of kmodel plots
int lw = 1;

// use bayesian blocks algorithm
bool use_bb = true;
int rebin = 10;

auto freec = 10000;

TH1D* _big(TH1D* h);
TH1* get_or_create_bb(TH1*);
void compute_and_save(TH1D* h, double scale, std::string name, bool force = false);
void compute_bayes_blocks();

void g_plot_K40(bool bayes = false) {

    // dummy histogram to extend the range
    auto _h0 = new TH1D("h", "h", 8000, 0, 8000);

    TH1D *h0, *h1, *h2, *h3, *h4, *h5;

    auto set1 = get_pdf_set("minishroud", "ms_all", "K40", true, "lar/");
    auto set2 = get_pdf_set("larveto", "outer_fibers", "K40", true, "lar/");
    auto set3 = get_pdf_set("larveto", "copper_shroud", "K40", true, "lar/");

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

    auto set1 = get_pdf_set("lar", "sur_array", "K42", true, "lar/");
    auto set2 = get_pdf_set("lar", "above_array", "K42", true, "lar/");
    auto set3 = get_pdf_set("gedet", "nplus_bege", "K42", true, "lar/");
    auto set4 = get_pdf_set("gedet", "nplus_semicoax", "K42", true, "lar/");

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
    auto set3 = get_pdf_set("larveto", "outer_fibers", "Bi212");
    auto set4 = get_pdf_set("larveto", "outer_fibers", "Tl208");

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
    // auto set3 = get_pdf_set("larveto", "outer_fibers", "Pb214");
    // auto set4 = get_pdf_set("larveto", "outer_fibers", "Bi214");

    if (!bayes) {
        auto& set1_M1 = set1.M1_enrBEGe;
        set1_M1->Add(set1.M1_enrCoax);
        set1_M1->Add(set2.M1_enrBEGe);
        set1_M1->Add(set2.M1_enrCoax);
        auto& set1_M2 = set1.M2_enrE1plusE2;
        set1_M2->Add(set2.M2_enrE1plusE2);

        // auto& set3_M1 = set3.M1_enrBEGe;
        // set3_M1->Add(set3.M1_enrCoax);
        // set3_M1->Add(set4.M1_enrBEGe);
        // set3_M1->Add(set4.M1_enrCoax);
        // auto& set3_M2 = set3.M2_enrE1plusE2;
        // set3_M2->Add(set4.M2_enrE1plusE2);

        h0 = set1_M1; h0->Rebin(rebin);
        h1 = set1_M2; h1->Rebin(rebin);
        // h2 = set3_M1; h2->Rebin(rebin);
        // h3 = set3_M2; h3->Rebin(rebin);
    }
    else {
        auto f = new TFile("bb-histos.root");
        h0 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_U_M1"));
        h1 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_U_M2"));
        // h2 = dynamic_cast<TH1D*>(f->Get("bb_larveto_fibers_U_M1"));
        // h3 = dynamic_cast<TH1D*>(f->Get("bb_larveto_fibers_U_M2"));
    }

    // props
    h0->GetXaxis()->SetRangeUser(560, 4999);
    h0->GetYaxis()->SetRangeUser(1.1E-10, 5E-03);
    h0->GetYaxis()->SetNdivisions(4);
    h0->SetTitle(";energy [keV];probability / (decay keV)");

    h0 -> SetLineColorAlpha(kTBriBlue, 1.0);
    h1 -> SetLineColorAlpha(kTBriBlue, 0.5);
    // h2 -> SetLineColorAlpha(kTBriRed,  1.0);
    // h3 -> SetLineColorAlpha(kTBriRed,  0.5);

    TLegend leg;
    leg.AddEntry(_big(h0), "^{214}Pb + ^{214}Bi - mini-shroud", "L");
    // leg.AddEntry(_big(h2), "^{214}Pb + ^{214}Bi - fibers", "L");

    // draw!
    TCanvas c;
    h0 -> Draw("HIST");
    h1 -> Draw("HIST SAME");
    // h2 -> Draw("HIST SAME");
    // h3 -> Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../gmodel-pdfs-U.pdf");

    set1.file->Close();
    set2.file->Close();
    // set3.file->Close();
    // set4.file->Close();
}

void g_plot_misc(bool bayes = false) {

    TH1D *h3, *h4, *h5, *h6;

    // auto set4 = get_pdf_set("minishroud", "ms_all", "Pa234m");
    auto set5 = get_pdf_set("minishroud", "ms_all", "Ac228");

    if (!bayes) {
        // auto& set4_M1 = set4.M1_enrBEGe;
        // set4_M1->Add(set4.M1_enrCoax);

        auto& set5_M1 = set5.M1_enrBEGe;
        set5_M1->Add(set5.M1_enrCoax);

        // h3 = set4_M1;             h3->Rebin(rebin);
        // h4 = set4.M2_enrE1plusE2; h4->Rebin(rebin);
        h5 = set5_M1;             h5->Rebin(rebin);
        h6 = set5.M2_enrE1plusE2; h6->Rebin(rebin);
    }
    else {
        auto f = new TFile("bb-histos.root");
        // h3 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_Pa234m_M1"));
        // h4 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_Pa234m_M2"));
        h5 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_Ac228_M1"));
        h6 = dynamic_cast<TH1D*>(f->Get("bb_minishroud_ms_all_Ac228_M2"));
    }

    // props
    h5->GetXaxis()->SetRangeUser(560, 2200);
    h5->GetYaxis()->SetNdivisions(4);
    h5->SetTitle(";energy [keV];probability / (decay keV)");

    // h3 -> SetLineColorAlpha(kTBriGreen,  1.0);
    // h4 -> SetLineColorAlpha(kTBriGreen,  0.5);
    h5 -> SetLineColorAlpha(kTBriPurple, 1.0);
    h6 -> SetLineColorAlpha(kTBriPurple, 0.5);

    TLegend leg(0.1, 0.14, 0.45, 0.34);
    // leg.AddEntry(_big(h3), "^{234m}Pa - mini-shroud", "L");
    leg.AddEntry(_big(h5), "^{228}Ac - mini-shroud",  "L");

    // draw!
    TCanvas c;
    h5 -> Draw("HIST SAME");
    h6 -> Draw("HIST SAME");
    // h3 -> Draw("HIST SAME");
    // h4 -> Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../gmodel-pdfs-misc.pdf");

    // set4.file->Close();
    set5.file->Close();
}

void g_plot_misc2(bool bayes = false) {

    TH1D *h0, *h1, *h2, *h3, *h4;

    auto set1 = get_pdf_set("cables", "cables_all", "Co60");
    auto set2 = get_pdf_set("gedet", "intrinsic_bege", "2nbb");
    auto set3 = get_pdf_set("gedet", "intrinsic_semicoax", "2nbb");

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

    // g_plot_K40(use_bb);
    // g_plot_K42(use_bb);
    // g_plot_Th(use_bb);
    // g_plot_U(use_bb);
    g_plot_misc(use_bb);
    g_plot_misc2(use_bb);

    std::cout << "All work completed." << std::endl;
}

TH1D* _big(TH1D* h) {
    auto H = dynamic_cast<TH1D*>(h->Clone());
    H->SetLineWidth(3);
    return H;
}

void compute_bayes_blocks() {

    // K42
    auto set1 = get_pdf_set("lar", "sur_array", "K42", false, "lar/");
    auto& set1_M1 = set1.M1_enrBEGe;
    set1_M1->Add(set1.M1_enrCoax);

    auto set2 = get_pdf_set("lar", "above_array", "K42", false, "lar/");
    auto& set2_M1 = set2.M1_enrBEGe;
    set2_M1->Add(set2.M1_enrCoax);

    auto set3 = get_pdf_set("gedet", "nplus_bege", "K42", false, "lar/");
    auto& set3_M1 = set3.M1_enrBEGe;
    set3_M1->Add(set3.M1_enrCoax);

    auto set4 = get_pdf_set("gedet", "nplus_semicoax", "K42", false, "lar/");
    auto& set4_M1 = set4.M1_enrBEGe;
    set4_M1->Add(set4.M1_enrCoax);

    compute_and_save(set1_M1            , set1.nedep, "bb_lar_sur_array_K42_M1"   );
    compute_and_save(set1.M2_enrE1plusE2, set1.ncoin, "bb_lar_sur_array_K42_M2"   );
    set1.file->Close();
    compute_and_save(set2_M1            , set2.nedep, "bb_lar_above_array_K42_M1" );
    compute_and_save(set2.M2_enrE1plusE2, set2.ncoin, "bb_lar_above_array_K42_M2" );
    set2.file->Close();
    compute_and_save(set3_M1            , set3.nedep, "bb_gedet_nplus_bege_K42_M1");
    compute_and_save(set3.M2_enrE1plusE2, set3.ncoin, "bb_gedet_nplus_bege_K42_M2");
    set3.file->Close();
    compute_and_save(set4_M1            , set4.nedep, "bb_gedet_nplus_coax_K42_M1");
    compute_and_save(set4.M2_enrE1plusE2, set4.ncoin, "bb_gedet_nplus_coax_K42_M2");
    set4.file->Close();

    // misc
    auto set5 = get_pdf_set("cables", "cables_all", "Co60", false, "lar/");
    auto& set5_M1 = set5.M1_enrBEGe;
    set5_M1->Add(set5.M1_enrCoax);

    auto set6 = get_pdf_set("gedet", "intrinsic_bege", "2nbb", false, "lar/");
    auto set7 = get_pdf_set("gedet", "intrinsic_semicoax", "2nbb", false, "lar/");
    auto& set6_M1 = set6.M1_enrBEGe;
    set6_M1->Add(set6.M1_enrCoax);
    set6_M1->Add(set7.M1_enrBEGe);
    set6_M1->Add(set7.M1_enrCoax);

    auto set9 = get_pdf_set("minishroud", "ms_all", "Ac228", false, "lar/");
    auto& set9_M1 = set9.M1_enrBEGe;
    set9_M1->Add(set9.M1_enrCoax);

    compute_and_save(set5_M1            , set5.nedep, "bb_cables_cables_all_Co60_M1"  );
    compute_and_save(set5.M2_enrE1plusE2, set5.ncoin, "bb_cables_cables_all_Co60_M2"  );
    set5.file->Close();
    compute_and_save(set6_M1            , set6.nedep + set7.nedep, "bb_gedet_intrinsic_2nbb_M1");
    set6.file->Close();
    set7.file->Close();
    compute_and_save(set9_M1            , set9.nedep, "bb_minishroud_ms_all_Ac228_M1" );
    compute_and_save(set9.M2_enrE1plusE2, set9.ncoin, "bb_minishroud_ms_all_Ac228_M2" );
    set9.file->Close();

    // K40
    auto set10 = get_pdf_set("minishroud", "ms_all", "K40", false, "lar/");
    auto& set10_M1 = set10.M1_enrBEGe;
    set10_M1->Add(set10.M1_enrCoax);

    auto set11 = get_pdf_set("larveto", "outer_fibers", "K40", false, "lar/");
    auto& set11_M1 = set11.M1_enrBEGe;
    set11_M1->Add(set11.M1_enrCoax);

    auto set12 = get_pdf_set("larveto", "copper_shroud", "K40", false, "lar/");
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
    auto set13 = get_pdf_set("minishroud", "ms_all", "Bi212", false, "lar/");
    auto set14 = get_pdf_set("minishroud", "ms_all", "Tl208", false, "lar/");
    auto& set13_M1 = set13.M1_enrBEGe;
    set13_M1->Add(set13.M1_enrCoax);
    set13_M1->Add(set14.M1_enrBEGe);
    set13_M1->Add(set14.M1_enrCoax);
    auto& set13_M2 = set13.M2_enrE1plusE2;
    set13_M2->Add(set14.M2_enrE1plusE2);

    auto set15 = get_pdf_set("larveto", "outer_fibers", "Bi212", false, "lar/");
    auto set16 = get_pdf_set("larveto", "outer_fibers", "Tl208", false, "lar/");
    auto& set15_M1 = set15.M1_enrBEGe;
    set15_M1->Add(set15.M1_enrCoax);
    set15_M1->Add(set16.M1_enrBEGe);
    set15_M1->Add(set16.M1_enrCoax);
    auto& set15_M2 = set15.M2_enrE1plusE2;
    set15_M2->Add(set16.M2_enrE1plusE2);

    compute_and_save(set13_M1            , set13.nedep + set14.nedep, "bb_minishroud_ms_all_Th_M1");
    compute_and_save(set13.M2_enrE1plusE2, set13.ncoin + set14.ncoin, "bb_minishroud_ms_all_Th_M2");
    set13.file->Close();
    set14.file->Close();
    compute_and_save(set15_M1            , set15.nedep + set16.nedep, "bb_larveto_fibers_Th_M1");
    compute_and_save(set15.M2_enrE1plusE2, set15.ncoin + set16.ncoin, "bb_larveto_fibers_Th_M2");
    set15.file->Close();
    set16.file->Close();

    // U
    auto set17 = get_pdf_set("minishroud", "ms_all", "Pb214", false, "lar/");
    auto set18 = get_pdf_set("minishroud", "ms_all", "Bi214", false, "lar/");
    auto& set17_M1 = set17.M1_enrBEGe;
    set17_M1->Add(set17.M1_enrCoax);
    set17_M1->Add(set18.M1_enrBEGe);
    set17_M1->Add(set18.M1_enrCoax);
    auto& set17_M2 = set17.M2_enrE1plusE2;
    set17_M2->Add(set18.M2_enrE1plusE2);

    // auto set19 = get_pdf_set("larveto", "outer_fibers", "Pb214", false, "lar/");
    // auto set20 = get_pdf_set("larveto", "outer_fibers", "Bi214", false, "lar/");
    // auto& set19_M1 = set19.M1_enrBEGe;
    // set19_M1->Add(set19.M1_enrCoax);
    // set19_M1->Add(set20.M1_enrBEGe);
    // set19_M1->Add(set20.M1_enrCoax);
    // auto& set19_M2 = set19.M2_enrE1plusE2;
    // set19_M2->Add(set20.M2_enrE1plusE2);

    compute_and_save(set17_M1            , set17.nedep + set18.nedep, "bb_minishroud_ms_all_U_M1");
    compute_and_save(set17.M2_enrE1plusE2, set17.ncoin + set18.ncoin, "bb_minishroud_ms_all_U_M2");
    set17.file->Close();
    set18.file->Close();
    // compute_and_save(set19_M1            , set19.nedep + set20.nedep, "bb_larveto_fibers_U_M1");
    // compute_and_save(set19.M2_enrE1plusE2, set19.ncoin + set20.ncoin, "bb_larveto_fibers_U_M2");
    // set19.file->Close();
    // set20.file->Close();
}

void compute_and_save(TH1D* h, double scale, std::string name, bool force) {
    // h->Scale(scale);
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

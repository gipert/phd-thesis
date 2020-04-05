/* pdfs_plot.C
 *
 * Author: Luigi Pertoldi - pertoldi@pd.infn.it
 * Created: Tue 1 Jan 2019
 *
 */

#include "../../../../../../tools/paul_tol_colors.hpp"
using namespace tolcols;
#include "../../../../../../tools/bayesian_blocks.hpp"
#include "../../../../../../tools/pdf_handling.hpp"
#include "kmodel_common.hpp"

// color alpha
double f_alpha = 0.02;

// line width
int lw = 2;

// use bayesian blocks algorithm
bool use_bb = false;
int rebin = 10;

auto freec = 10000;

TH1D* _big(TH1D* h);
TH1D* remove_bin(TH1D* h);

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
    h0->GetYaxis()->SetRangeUser(2E-08, 2E-03);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.004);
    // h0->GetYaxis()->SetTitleOffset(1.8);
    // h0->GetXaxis()->SetTickSize(0.01);

    h0->SetLineColor(kTBriBlue);
    h1->SetLineColor(kTBriCyan);
    h2->SetLineColor(kTBriPurple);
    h3->SetLineColor(kTBriRed);
    h4->SetLineColor(kTBriGreen);
    h5->SetLineColor(kTMutIndigo);
    h6->SetLineColor(kTBriYellow);
    h7->SetLineColor(kTBriCyan);

    for (auto i : {h0, h1, h2, h3, h4, h5, h6, h7}) {
        i->SetFillStyle(1001);
        i->SetLineWidth(lw);
        i->SetFillColorAlpha(i->GetLineColor(), f_alpha);
    }

    TLegend leg(0.1, 0.8, 0.98, 0.97);
    leg.SetFillColorAlpha(0,0);
    leg.SetNColumns(3);
    // leg.AddEntry(_big(h1), "^{40}K cables", "L");
    leg.AddEntry(_big(h0), "^{40}K holders", "L");
    leg.AddEntry(_big(h2), "^{40}K mini-shroud", "L");
    leg.AddEntry(_big(h3), "^{40}K fiber shroud", "L");
    leg.AddEntry(_big(h4), "^{40}K copper shroud", "L");
    leg.AddEntry(_big(h6), "^{40}K SiPMs", "L");
    leg.AddEntry(_big(h7), "^{40}K front-end elec.", "L");
    leg.AddEntry(_big(h5), "2#nu#beta#beta", "L");
    // draw!
    TCanvas c;
    c.GetPad(0)->SetBottomMargin(0.15);
    c.GetPad(0)->SetLeftMargin(0.095);
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
    c.SaveAs("../kmodel-pdfs-K40-long.pdf");

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
    h0->GetXaxis()->SetLabelOffset(0.004);
//    h0->GetYaxis()->SetTitleOffset(1.8);
//    h0->GetXaxis()->SetTickSize(0.01);

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

    TLegend leg(0.35, 0.8, 0.73, 0.97);
    leg.SetNColumns(3);
    leg.SetHeader("^{40}K close to mini-shroud no.", "L");
    leg.AddEntry(_big(h1), "1", "L");
    leg.AddEntry(_big(h4), "4", "L");
    leg.AddEntry(_big(h6), "6", "L");
    leg.AddEntry(_big(h2), "2", "L");
    leg.AddEntry(_big(h5), "5", "L");
    leg.AddEntry(_big(h0), "7", "L");
    leg.AddEntry(_big(h3), "3", "L");

    // draw!
    TCanvas c;
    c.GetPad(0)->SetBottomMargin(0.15);
    c.GetPad(0)->SetLeftMargin(0.095);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h5->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K40-sep-long.pdf");

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
    h0->GetYaxis()->SetRangeUser(2E-09, 0.99E-02);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.004);

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

    TLegend leg(0.35, 0.82, 0.95, 0.97);
    leg.SetFillColorAlpha(0,0);
    leg.SetNColumns(2);
    // leg.AddEntry(_big(h1), "^{40}K cables", "L");
    leg.AddEntry(_big(h0), "^{42}K LAr hom.", "L");
    leg.AddEntry(_big(h1), "^{42}K LAr above", "L");
    leg.AddEntry(_big(h2), "^{42}K n^{+} BEGe", "L");
    leg.AddEntry(_big(h3), "^{42}K n^{+} Coax", "L");
    leg.AddEntry(_big(h4), "2#nu#beta#beta", "L");

    // draw!
    TCanvas c;
    c.GetPad(0)->SetBottomMargin(0.15);
    c.GetPad(0)->SetLeftMargin(0.095);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K42-long.pdf");

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
    h0->GetYaxis()->SetRangeUser(1.1E-08, 2E-02);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.004);
//    h0->GetYaxis()->SetTitleOffset(1.8);
//    h0->GetXaxis()->SetTickSize(0.01);

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

    TLegend leg(0.1, 0.85, 0.9, 0.97);
    leg.SetHeader("^{42}K above mini-shroud no.", "L");
    leg.SetNColumns(7);
    leg.AddEntry(_big(h0), "1", "L");
    leg.AddEntry(_big(h1), "2", "L");
    leg.AddEntry(_big(h2), "3", "L");
    leg.AddEntry(_big(h3), "4", "L");
    leg.AddEntry(_big(h4), "5", "L");
    leg.AddEntry(_big(h5), "6", "L");
    leg.AddEntry(_big(h6), "7", "L");

    // draw!
    TCanvas c;
    c.GetPad(0)->SetBottomMargin(0.15);
    c.GetPad(0)->SetLeftMargin(0.095);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h5->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K42-sep-long.pdf");

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
    h0->GetYaxis()->SetRangeUser(2E-10, 0.9E-03);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.004);
//    h0->GetYaxis()->SetTitleOffset(1.8);
//    h0->GetXaxis()->SetTickSize(0.01);

    h0->SetLineColor(kTBriBlue);
    h1->SetLineColor(kTBriCyan);
    h2->SetLineColor(kTBriPurple);
    h3->SetLineColor(kTBriRed);
    h4->SetLineColor(kTBriGreen);
    h6->SetLineColor(kTBriYellow);
    h7->SetLineColor(kTBriCyan);

    for (auto i : {h0, h1, h2, h3, h4, h6, h7}) {
        i->SetFillStyle(1001);
        i->SetLineWidth(lw);
        i->SetFillColorAlpha(i->GetLineColor(), f_alpha);
    }

    TLegend leg(0.1, 0.85, 0.98, 0.97);
    leg.SetNColumns(3);
    // leg.AddEntry(_big(h1), "^{40}K cables", "L");
    leg.AddEntry(_big(h0), "^{40}K holders", "L");
    leg.AddEntry(_big(h2), "^{40}K mini-shroud", "L");
    leg.AddEntry(_big(h3), "^{40}K fiber shroud", "L");
    leg.AddEntry(_big(h4), "^{40}K copper shroud", "L");
    leg.AddEntry(_big(h6), "^{40}K SiPMs", "L");
    leg.AddEntry(_big(h7), "^{40}K front-end elec.", "L");

    // draw!
    TCanvas c;
    c.GetPad(0)->SetBottomMargin(0.15);
    c.GetPad(0)->SetLeftMargin(0.095);
    h0->Draw("HIST");
    //h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    h7->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K40-M2-long.pdf");

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
    h0->GetXaxis()->SetLabelOffset(0.004);
//    h0->GetYaxis()->SetTitleOffset(1.8);
//    h0->GetXaxis()->SetTickSize(0.01);

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

    TLegend leg(0.1, 0.86, 0.7, 0.96);
    // leg.AddEntry(_big(h1), "^{40}K cables", "L");
    leg.SetNColumns(2);
    leg.AddEntry(_big(h0), "^{42}K LAr hom.", "L");
    leg.AddEntry(_big(h1), "^{42}K LAr above", "L");
    leg.AddEntry(_big(h2), "^{42}K n^{+} BEGe", "L");
    leg.AddEntry(_big(h3), "^{42}K n^{+} Coax", "L");

    // draw!
    TCanvas c;
    c.GetPad(0)->SetBottomMargin(0.15);
    c.GetPad(0)->SetLeftMargin(0.095);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K42-M2-long.pdf");

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
    h0->GetYaxis()->SetRangeUser(1.5E-07, 0.6E-03);
    h0->SetTitle(";;probability / (decay ROI channel)");
    // set x-axis labels
    for (int b = 1; b <= h0->GetNbinsX(); ++b) {
        h0->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
        // h0->GetXaxis()->SetBinLabel(b, (det[b-1]).c_str());
    }
    h0->GetXaxis()->LabelsOption("v");
    h0->GetXaxis()->SetLabelOffset(0.004);
//    h0->GetYaxis()->SetTitleOffset(1.8);
//    h0->GetXaxis()->SetTickSize(0.01);

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

    TLegend leg(0.1, 0.87, 0.9, 0.97);
    leg.SetHeader("^{40}K close to mini-shroud no.", "L");
    leg.SetNColumns(7);
    leg.AddEntry(_big(h1), "1", "L");
    leg.AddEntry(_big(h2), "2", "L");
    leg.AddEntry(_big(h3), "3", "L");
    leg.AddEntry(_big(h4), "4", "L");
    leg.AddEntry(_big(h5), "5", "L");
    leg.AddEntry(_big(h6), "6", "L");
    leg.AddEntry(_big(h0), "7", "L");

    // draw!
    TCanvas c;
    c.GetPad(0)->SetBottomMargin(0.15);
    c.GetPad(0)->SetLeftMargin(0.095);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h5->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K40-sep-M2-long.pdf");

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
    h0->GetXaxis()->SetLabelOffset(0.004);
//    h0->GetYaxis()->SetTitleOffset(1.8);
//    h0->GetXaxis()->SetTickSize(0.01);

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

    TLegend leg(0.1, 0.8, 0.98, 0.97);
    leg.SetHeader("^{42}K above mini-shroud no.", "L");
    leg.SetNColumns(3);
    leg.AddEntry(_big(h0), "1", "L");
    leg.AddEntry(_big(h1), "2", "L");
    leg.AddEntry(_big(h2), "3", "L");
    leg.AddEntry(_big(h3), "4", "L");
    leg.AddEntry(_big(h4), "5", "L");
    leg.AddEntry(_big(h5), "6", "L");
    leg.AddEntry(_big(h6), "7", "L");

    // draw!
    TCanvas c;
    c.GetPad(0)->SetBottomMargin(0.15);
    c.GetPad(0)->SetLeftMargin(0.095);
    h0->Draw("HIST");
    h1->Draw("HIST SAME");
    h2->Draw("HIST SAME");
    h3->Draw("HIST SAME");
    h4->Draw("HIST SAME");
    h5->Draw("HIST SAME");
    h6->Draw("HIST SAME");
    leg.Draw();

    gPad->SetLogy();
    c.SaveAs("../kmodel-pdfs-K42-sep-M2-long.pdf");

    set1.file->Close();
    set2.file->Close();
    set3.file->Close();
    set4.file->Close();
    set5.file->Close();
    set6.file->Close();
    set7.file->Close();
}

void kpdfs_plot_long() {

    tolcols::init();

    gROOT->Macro("gerda-style-short.C");
    gStyle->SetLineScalePS(2.2);

    gROOT->ForceStyle();
    gROOT->SetBatch();

    float f = 10;
    for (int i = 0; i < 11; ++i) new TColor(freec+i, (0+i*f)/255., (51+i*f)/255., (102+i*f)/255., "blue");

    k_plot_K40();
    k_plot_K42();
    k_plot_K40_sep();
    k_plot_K42_sep();

    k_plot_K40_M2();
    k_plot_K42_M2();
    k_plot_K40_sep_M2();
    k_plot_K42_sep_M2();

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

// vim: foldmethod=syntax

/* kmodel_regions.C
 *
 * Author: Luigi Pertoldi
 * Created: 26 Dec 2018
 *
 */

#include "../../../../tools/paul_tol_colors.hpp"
using namespace tolcols;

void kmodel_regions() {

    gROOT->Macro("../../../../tools/gerda-style-long.C");
    tolcols::init();

    gROOT->ForceStyle();
    gROOT->SetBatch();

    // get histograms
    auto f     = new TFile("results/gmodel/gmodel-results.root");
    auto data_enrBEGe  = dynamic_cast<TH1D*>(f->Get("histos/hdata_ch1"));
    auto data_enrCoax  = dynamic_cast<TH1D*>(f->Get("histos/hdata_ch2"));
    auto data_E1plusE2 = dynamic_cast<TH1D*>(f->Get("histos/hdata_ch0"));

    /* ---------------------------------------------------------------------- */
    // set histogram properties
    data_enrBEGe->GetXaxis()->SetRangeUser(1401, 1587);
    data_enrBEGe->GetYaxis()->SetRangeUser(0.5, 2000);

    data_enrBEGe->SetTitle(";energy [keV];counts / 1 keV");
    data_enrBEGe->SetLineColor(kTBriBlue);
    data_enrCoax->SetLineColor(kTBriRed);
    data_E1plusE2->SetLineColor(kTBriGreen);

    // lines
    auto K40 = new TBox(1457, data_enrBEGe->GetMinimum(), 1465, data_enrBEGe->GetMaximum());
    auto K42 = new TBox(1521, data_enrBEGe->GetMinimum(), 1529, data_enrBEGe->GetMaximum());
    auto SB1 = new TBox(1405, data_enrBEGe->GetMinimum(), 1450, data_enrBEGe->GetMaximum());
    auto SB2 = new TBox(1470, data_enrBEGe->GetMinimum(), 1515, data_enrBEGe->GetMaximum());
    auto SB3 = new TBox(1535, data_enrBEGe->GetMinimum(), 1580, data_enrBEGe->GetMaximum());

    // text
    auto TK40 = new TLatex((1465+1457)/2, 5, "#font[82]{K40}");
    auto TK42 = new TLatex((1529+1521)/2, 5, "#font[82]{K42}");
    auto TSB1 = new TLatex((1450+1405)/2, 50, "#font[82]{SB1}");
    auto TSB2 = new TLatex((1515+1470)/2, 30, "#font[82]{SB2}");
    auto TSB3 = new TLatex((1580+1535)/2, 20, "#font[82]{SB3}");

    // legend
    TLegend * leg = new TLegend(0.08, 0.70, 0.4, 0.97, "exposure: 60.2 kg#upointyr");
    leg->SetFillStyle(0);
    leg->SetNColumns(1);
    leg->SetMargin(0.15);
    auto tmp = dynamic_cast<TH1D*>(data_enrBEGe->Clone());
    tmp->SetLineWidth(4);
    leg->AddEntry(tmp, "M1-enrBEGe", "L");
    tmp = dynamic_cast<TH1D*>(data_enrCoax->Clone());
    tmp->SetLineWidth(4);
    leg->AddEntry(tmp, "M1-enrCoax", "L");
    tmp = dynamic_cast<TH1D*>(data_E1plusE2->Clone());
    tmp->SetLineWidth(4);
    leg->AddEntry(tmp, "M2-enrGe", "L");

    // GERDA watermark
    auto gerdawtr = new TLatex(0.992, 0.78, "GERDA 19-09");
    gerdawtr->SetNDC();
    gerdawtr->SetTextFont(43);
    gerdawtr->SetTextSizePixels(16);
    gerdawtr->SetTextAngle(90);

    // Draw!
    auto canvas = new TCanvas();
    gPad->SetLogy();
    data_enrBEGe->Draw();
    data_enrCoax->Draw("SAME HIST");
    data_E1plusE2->Draw("SAME HIST");
    for (auto p : {K40, K42, SB1, SB2, SB3}) {
        p->SetFillColorAlpha(kTBriGrey, 0.2);
        p->SetLineColorAlpha(kTBriGrey, 0.7);
        p->Draw("L SAME");
    }
    for (auto p : {TK40, TK42, TSB1, TSB2, TSB3}) {
        p->SetTextAlign(22);
        p->SetTextSizePixels(30);
        // p->SetFillColorAlpha(PTColor["kTBriGrey"], 0.2);
        // p->SetLineColorAlpha(PTColor["kTBriGrey"], 0.7);
        p->Draw("SAME");
    }
    leg->Draw();
    gerdawtr->Draw();
    data_enrBEGe->Draw("SAME AXIS");

    canvas->SaveAs("kmodel-regions.pdf");

    return;
}

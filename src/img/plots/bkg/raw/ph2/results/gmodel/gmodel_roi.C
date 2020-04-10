/* gmodel_roi.C
 *
 * Author: Luigi Pertoldi
 * Created: 30 Jun 2018
 *
 */

#include "gmodel_common.hpp"

void plot_gmodel(int dataset = 0,
                 double xrangelow = 1930,
                 double xrangeup  = 2190,
                 double yrangelow = 0.002,
                 double yrangeup  = 12,
                 int rebin = 2) {

    // get histograms
    auto ds    = datdict[dataset];
    auto f     = new TFile("gmodel-results.root");
    auto fd    = new TFile("gerda-data-orig.root");
    auto data  = dynamic_cast<TH1D*>(fd->Get(ds.c_str()));
    auto total = dynamic_cast<TH1D*>(f->Get(Form("histos/hmodel_ch%d", dataset)));
    TTreeReader reader("fit_result_sorted", f);
    TTreeReaderValue<double> marg_mode(reader, "marginalized_mode");

    // retrieve components
    std::vector<TH1D*> comp;
    for (auto& n : cnames) {
        auto h_begin = dynamic_cast<TH1D*>(data->Clone());
        h_begin->Reset();
        comp.push_back(h_begin);
        for (auto& p : n.second) {
            auto h = dynamic_cast<TH1D*>(f->Get(Form("histos/h%d_ch%d", p, dataset)));
            if (h) {
                // scale histograms with fit results
                reader.SetEntry(p);
                h->Scale(*marg_mode);

                comp.back()->Add(h);
            }
        }
        comp.back()->SetName(n.first.c_str());
    }
    for (auto it = comp.begin(); it != comp.end(); ++it) if (!(*it)) comp.erase(it);

    // rebin
    data ->Rebin(rebin);
    total->Rebin(rebin); total->SetLineWidth(2);
    for (auto& h : comp) {
        h->Rebin(rebin);
        h->SetLineWidth(2);
    }

    /* ---------------------------------------------------------------------- */
    // set histogram properties
    data->SetTitle(Form(";energy [keV];counts / %d keV", rebin));
    data->GetXaxis()->SetTitleOffset(1);
    data->SetFillStyle(1001);
    data->SetLineColorAlpha(kTVibBlue, 0.3);
    data->SetFillColorAlpha(kTVibBlue, 0.2);
    data->GetXaxis()->SetRangeUser(xrangelow, xrangeup);
    data->GetYaxis()->SetRangeUser(yrangelow, yrangeup);
    // data->GetXaxis()->SetNdivisions(509);

    for (auto& h : comp) {
        h->SetLineColor(hcolor[h->GetName()]);
//        h->SetFillColorAlpha(hcolor[h->GetName()], 0.4);
//        h->SetFillStyle(1001);
    }

    // legend
    TLegend * leg;
    if(ds=="enrBEGe") leg  = new TLegend(0.6, 0.70, 0.98, 0.97,
                                         Form("#font[82]{M1-enrBEGe} - %0.1f kg#upointyr", exposure[ds]));
    if(ds=="enrCoax") leg = new TLegend(0.6, 0.70, 0.98, 0.97,
                                        Form("#font[82]{M1-enrCoax} - %0.1f kg#upointyr", exposure[ds]));

    leg->SetNColumns(4);
    leg->AddEntry(data, "Data", "F");
    auto tmp = dynamic_cast<TH1D*>( total->Clone() );
    tmp->SetLineWidth(4);
    leg->AddEntry(tmp, "Model", "L");
    for (auto& h: comp) {
        tmp = dynamic_cast<TH1D*>(h->Clone());
        tmp->SetLineWidth(4);
        leg->AddEntry(tmp, nice_names[h->GetName()].c_str(), "L");
    }
    leg->SetMargin(0.3);

    // graphics settings
    auto canvas = new TCanvas(Form("gmodel_canvas_%s", ds.c_str()), "canvas for g-model results");

    // GERDA watermark
    auto gerdawtr = new TLatex(0.99, 0.81, "GERDA 19-09");
    gerdawtr->SetNDC();
    gerdawtr->SetTextFont(43);
    gerdawtr->SetTextSizePixels(16);
    gerdawtr->SetTextAngle(90);

    // ROI box
    int low_edge = data->GetXaxis()->GetBinLowEdge(data->GetXaxis()->FindBin(2014));
    int up_edge = data->GetXaxis()->GetBinLowEdge(data->GetXaxis()->FindBin(2064));
    auto box = new TBox(low_edge, data->GetMinimum(), up_edge, data->GetMaximum());
    box->SetFillColorAlpha(kGray, 1); // this not to cover the overlaying histograms
    box->SetFillStyle(1001);
    box->SetLineWidth(0);
    auto line = new TLine(2039, data->GetMinimum(), 2039, data->GetMaximum());
    auto low_line = new TLine(low_edge, data->GetMinimum(), low_edge, data->GetMaximum());
    auto up_line = new TLine(up_edge, data->GetMinimum(), up_edge, data->GetMaximum());
    for (auto l : {line, low_line, up_line}) l->SetLineWidth(2);
    up_line->SetLineStyle(2);
    low_line->SetLineStyle(2);

    string exposure;
    if (ds == "enrBEGe") exposure = "32.1";
    if (ds == "enrCoax") exposure = "28.1";

    TLatex * ttext;
    if (ds == "enrBEGe") ttext = new TLatex(0.59, 0.93, ("#font[82]{M1-enrBEGe} - " + exposure + " kg yr").c_str());
    if (ds == "enrCoax") ttext = new TLatex(0.59, 0.93, ("#font[82]{M1-enrCoax} - " + exposure + " kg yr").c_str());
    ttext->SetNDC();

    // Draw!
    canvas->cd();
    gPad->SetBottomMargin(0.11);
    gPad->SetLeftMargin(0.08);
    canvas->SetLogy();
    data->Draw("HIST");
    total->Draw("SAME HIST");
    for (auto& h : comp) h->Draw("SAME HIST");
    // leg->Draw();
    low_line->Draw();
    up_line->Draw();
    // line->Draw();
    gerdawtr->Draw();
    ttext->Draw();

    canvas->SaveAs(Form("gmodel-roi-%s.pdf" , ds.c_str()));

    return;
}

void gmodel_roi() {

    tolcols::init();
    gROOT->Macro("../../../../../../tools/gerda-style-short.C");
    gStyle->SetLineScalePS(1.2);

    gROOT->ForceStyle();
    gROOT->SetBatch();
    plot_gmodel(1);
    plot_gmodel(2);
}

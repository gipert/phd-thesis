/* amodel_plot.C
 *
 * Author: Luigi Pertoldi
 * Created: 30 Jun 2018
 *
 */

#include "amodel_common.hpp"

void plot_amodel(int dataset = 0, int overflowsize = 5) {

    int rebin = 10;

    double xrangelow = 3500;
    double xrangeup  = 5250;
    double overflow  = 8000;
    double yrangelow = 0.02;
    double yrangeup  = 1.4E3;

    // get histograms
    auto ds    = datdict[dataset];
    auto f     = new TFile(Form("amodel-%s-results.root", ds.c_str()));
    auto data  = dynamic_cast<TH1D*>(f->Get(Form("results_canvas/hSum_all_%s", ds.c_str())));
    auto total = dynamic_cast<TH1D*>(f->Get(Form("results_canvas/hMC_all_%s", ds.c_str())));

    // retrieve components
    std::vector<TH1D*> comp;
    int offset_index = -1, slope_index = -1, ind = 0;
    for (auto& n : cnames) {
        auto h = dynamic_cast<TH1D*>(f->Get(Form("components/%s_all_%s", n.c_str(), ds.c_str())));
        if (h) {
            comp.push_back(h);
            hcolor.insert({Form("%s_all_%s", n.c_str(), ds.c_str()), hcolor[n]});
            nice_names.insert({Form("%s_all_%s", n.c_str(), ds.c_str()), nice_names[n]});
            if( n.find("offset") != string::npos ) offset_index = ind;
            if( n.find("slope") != string::npos ) slope_index = ind;
            ind++;
        }
    }

    // rebin
    data ->Rebin(rebin);
    total->Rebin(rebin); total->SetLineWidth(2);
    for (auto h : comp)
    {
        h->Rebin(rebin);
        h->SetLineWidth(2);
    }

    // put together degraded alpha contribution
    if(offset_index >= 0 && slope_index >= 0)
    {
        comp[offset_index]->Add(comp[slope_index]);
        comp.erase(comp.begin() + slope_index);
    }

    // put everything above 5250 keV into the overflow bin
    int of_bin = data->FindBin(xrangeup + 0.5*rebin);
    int nbins = data->GetNbinsX();
    for(int i = of_bin + 1; i < nbins; i++)
    {
        double data_cof = data->GetBinContent(of_bin);
        data->SetBinContent(of_bin, data_cof + data->GetBinContent(i));

        double mc_cof = total->GetBinContent(of_bin);
        total->SetBinContent(of_bin, mc_cof + total->GetBinContent(i));

        for (auto h : comp)
        {
            double h_cof = h->GetBinContent(of_bin);
            h->SetBinContent(of_bin, h_cof + h->GetBinContent(i));
        }
    }

    for (int i = of_bin + 1; i < nbins; i++) {
        data->SetBinContent(i, data->GetBinContent(of_bin));
        total->SetBinContent(i, total->GetBinContent(of_bin));
        for (auto h : comp) h->SetBinContent(i, h->GetBinContent(of_bin));
    }

    // compute residuals
    auto res     = dynamic_cast<TH1D*>(data->Clone("h_res"));
    auto res_b3u = dynamic_cast<TH1D*>(res->Clone("h_band3_up"));
    auto res_b3l = dynamic_cast<TH1D*>(res_b3u->Clone("h_band3_low"));
    auto res_b2u = dynamic_cast<TH1D*>(res_b3u->Clone("h_band2_up"));
    auto res_b2l = dynamic_cast<TH1D*>(res_b3u->Clone("h_band2_low"));
    auto res_b1u = dynamic_cast<TH1D*>(res_b3u->Clone("h_band1_up"));
    auto res_b1l = dynamic_cast<TH1D*>(res_b3u->Clone("h_band1_low"));

    TF1 fu("f", "TMath::Poisson(x,[0])", 0, 15);
    fu.SetNpx(1000);

    auto median = [](double x) {

        double up = x + 1./3;
        double dw = x - log(2.) > 0 ? x - log(2.) : 0;
        double mprec = 0.01;

        TF1 f_tmp("f_tmp", "TMath::Poisson(x,[0])", 0, 1);
        f_tmp.SetParameter(0, x);
        f_tmp.SetNpx(1000);
        double norm = f_tmp.Integral(0, x < 1 ? 5 : 10*x);

        double I = f_tmp.Integral(0, dw)/norm;
        while (I < 0.5) {
            dw += mprec;
            I = f_tmp.Integral(0, dw)/norm;
            if (dw > up) {
                std::cout << "WARNING: failed to calculate median" << std::endl;
                break;
            }
        }

        return dw;
    };

    int bin_min = res->GetXaxis()->FindBin(xrangelow);
    int bin_max = res->GetXaxis()->FindBin(xrangeup+overflowsize*rebin);

    for (int b = bin_min; b <= bin_max; b++) {
        double d = data->GetBinContent(b);
        double m = total->GetBinContent(b);
        double s;

        if (m < 50) {
            fu.SetParameter(0, m);

            double prob = fu.Integral(median(m), d);
            if (abs(prob) >= 0.5) {
                std::cout << "WARNING: poisson_area(" << d << ", " << median(m) << ") = " << prob << " in bin " << b << std::endl;
                s = prob == 1 ? 3.90 : -3.9;
            }
            else s = TMath::NormQuantile(0.5+prob);
        }
        else {
            s = (d-m)/sqrt(m);
        }

        if (b < bin_max-overflowsize || b == bin_max-overflowsize/2-1){
            res->SetBinContent(b, s);
        }
        else res->SetBinContent(b, 10);
        res_b3u->SetBinContent(b, +3);
        res_b3l->SetBinContent(b, -3);
        res_b2u->SetBinContent(b, +2);
        res_b2l->SetBinContent(b, -2);
        res_b1u->SetBinContent(b, +1);
        res_b1l->SetBinContent(b, -1);
    }

    /* ---------------------------------------------------------------------- */
    // set histogram properties
    data->SetTitle(Form(";;counts / %d keV", rebin));
    data->SetLabelSize(0, "X");
    data->SetFillStyle(1001);
    data->SetLineColorAlpha(kTVibBlue, 0.2);
    data->SetFillColorAlpha(kTVibBlue, 0.2);
    data->GetXaxis()->SetRangeUser(xrangelow, xrangeup+overflowsize*rebin);
    data->GetYaxis()->SetRangeUser(yrangelow, yrangeup);
    data->GetXaxis()->SetNdivisions(509);

    for (auto& h : comp) {
        h->SetLineColor(hcolor[h->GetName()]);
//        h->SetFillColorAlpha(hcolor[h->GetName()], 0.4);
//        h->SetFillStyle(1001);
    }

    res->SetMarkerStyle(20);
    res->SetMarkerSize(0.5);

    for (auto& h : {res, res_b3u, res_b3l, res_b2u, res_b2l, res_b1u, res_b1l}) {
        h->SetLineWidth(0);
//        h->GetXaxis()->SetNdivisions(res->GetNbinsX(), 0, 0);
        h->GetYaxis()->SetNdivisions(8, 0, 0);
    }
    res_b3u->GetXaxis()->SetRangeUser(xrangelow, xrangeup+overflowsize*rebin);
    res_b3u->GetYaxis()->SetRangeUser(-3.99, 3.99);
    res_b3u->SetTitle(";energy [keV];residuals");
    res_b3u->GetYaxis()->SetNdivisions(507);
    res_b3u->GetXaxis()->SetNdivisions(509);
    res_b3u->SetTickLength(0.02, "X");
    res_b3u->SetFillColor(kTLigOrange);
    res_b3l->SetFillColor(kTLigOrange);
    res_b2u->SetFillColor(kTLigLightYellow);
    res_b2l->SetFillColor(kTLigLightYellow);
    res_b1u->SetFillColor(kTLigPear);
    res_b1l->SetFillColor(kTLigPear);

    // legend
    TLegend * leg;
    if(ds=="enrBEGe") leg = new TLegend(0.08, 0.55, 0.62, 0.96,
                                        Form("^{enr}BEGe - %.1f kg#upointyr", exposure[ds]));
    if(ds=="enrCoax") leg = new TLegend(0.08, 0.51, 0.62, 0.96,
                                        Form("^{enr}Coax - %.1f kg#upointyr", exposure[ds]));
    if(ds=="natCoax") leg = new TLegend(0.08, 0.51, 0.62, 0.96,
                                        Form("^{nat}Coax - %.1f kg#upointyr", exposure[ds]));

    leg->SetNColumns(2);
    leg->AddEntry(data, "Data", "F");
    auto tmp = dynamic_cast<TH1D*>( total->Clone() );
    tmp->SetLineWidth(4);
    leg->AddEntry(tmp, "Model", "L");
    for (auto& h: comp) {
        tmp = dynamic_cast<TH1D*>(h->Clone());
        tmp->SetLineWidth(4);
        leg->AddEntry(tmp, nice_names[h->GetName()].c_str(), "L");
    }
    leg->SetMargin(0.15);

    // graphics settings
    auto canvas = new TCanvas(Form("amodel_canvas_%s", ds.c_str()), "canvas for a-model results");
    auto dataPad = new TPad("dataPad", "pad for the components", 0.0, 0.3, 1.0, 1.0);
    auto resPad = new TPad("resPad", "pad for the residuals", 0.0, 0.0, 1.0, 0.3);
    dataPad->SetBottomMargin(0.01);
    dataPad->SetLogy();
    resPad->SetTopMargin(0.01);
    resPad->SetTicks(1,1);
//    resPad->SetGridx();

    // GERDA watermark
    auto gerdawtr = new TLatex(0.973, 0.715, "GERDA 19-09");
    gerdawtr->SetNDC();
    gerdawtr->SetTextFont(43);
    gerdawtr->SetTextSizePixels(15);
    gerdawtr->SetTextAngle(90);

    // separate overflow bin
    TLine ldata(xrangeup,yrangelow,xrangeup,yrangeup);
    TLine lres(xrangeup,-3.99,xrangeup,+3.99);
    ldata.SetLineWidth(2); lres.SetLineWidth(2);
    ldata.SetLineColor(kGray+2); lres.SetLineColor(kGray+2);

    // Draw!
    canvas->cd();
    dataPad->Draw();
    dataPad->cd();
    data->Draw();
    for (auto& h : comp) h->Draw("SAME HIST");
    total->Draw("SAME HIST");
    leg->Draw();
    gerdawtr->Draw();
    ldata.Draw();

    canvas->cd();
    resPad->Draw();
    resPad->cd();
    res_b3u->Draw();
    res_b2u->Draw("SAME");
    res_b1u->Draw("SAME");
    res_b3l->Draw("SAME");
    res_b2l->Draw("SAME");
    res_b1l->Draw("SAME");
    res->Draw("P SAME");
    resPad->RedrawAxis("");
    resPad->RedrawAxis("G");
    lres.Draw();

    canvas->Update();
    canvas->SaveAs(Form("amodel-%s.pdf" , ds.c_str()));

    return;
}

void amodel_plot() {

    tolcols::init();
    gROOT->Macro("gerda-style-amodel.C");

    gROOT->ForceStyle();
    gROOT->SetBatch();
    plot_amodel(0,3);
    plot_amodel(1,3);
    plot_amodel(2,3);
}

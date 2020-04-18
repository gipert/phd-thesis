/* gmodel_plot.C
 *
 * Author: Luigi Pertoldi
 * Created: 30 Jun 2018
 *
 */

#include "gmodel_common.hpp"

void plot_gmodel(int dataset = 0,
                 double xrangelow = 560,
                 double xrangeup  = 5300,
                 double yrangelow = 0.02,
                 double yrangeup  = 5E3,
                 int rebin = 10) {

    // get histograms
    auto ds    = datdict[dataset];
    auto f     = new TFile("gmodel-results.root");
    auto data  = dynamic_cast<TH1D*>(f->Get(Form("histos/hdata_ch%d", dataset)));
    auto total = dynamic_cast<TH1D*>(f->Get(Form("histos/hmodel_ch%d", dataset)));
    TTreeReader reader("fit_result_sorted", f);
    TTreeReaderValue<double> marg_mode(reader, "marginalized_mode");

    // retrieve components
    std::vector<TH1D*> comp;
    for (auto& n : cnames) {
        if (dataset == 0 && (n.first == "alpha" || n.first == "2nbb")) continue;
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

	int n_tot = 0, n_1sigma = 0, n_2sigma = 0, n_3sigma = 0;

    for (int b = res->GetXaxis()->FindBin(xrangelow);
             b <= res->GetXaxis()->FindBin(xrangeup);
             ++b) {
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

        res->SetBinContent(b, s);
        res_b3u->SetBinContent(b, +3);
        res_b3l->SetBinContent(b, -3);
        res_b2u->SetBinContent(b, +2);
        res_b2l->SetBinContent(b, -2);
        res_b1u->SetBinContent(b, +1);
        res_b1l->SetBinContent(b, -1);

    }

    // clean ROI not for M2 data set!
    if (dataset != 0) {
      for (int b = 1; b <= data->GetNbinsX(); ++b) {
        if (b >= data->GetXaxis()->FindBin(2014) and
            b < data->GetXaxis()->FindBin(2064)) {
              data   ->SetBinContent(b, 0);
              res    ->SetBinContent(b, 0);
              res_b3u->SetBinContent(b, 0);
              res_b2u->SetBinContent(b, 0);
              res_b1u->SetBinContent(b, 0);
              res_b3l->SetBinContent(b, 0);
              res_b2l->SetBinContent(b, 0);
              res_b1l->SetBinContent(b, 0);
        }
      }
    }

    /* ---------------------------------------------------------------------- */
    // set histogram properties
    data->SetTitle(Form(";;counts / %d keV", rebin));
    data->SetLabelSize(0, "X");
    data->SetFillStyle(1001);
    data->SetLineColorAlpha(kTVibBlue, 0.3);
    data->SetFillColorAlpha(kTVibBlue, 0.2);
    data->GetXaxis()->SetRangeUser(xrangelow, xrangeup);
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
        h->GetYaxis()->SetNdivisions(4, 0, 0);
    }
    res_b3u->GetXaxis()->SetRangeUser(xrangelow, xrangeup);
    res_b3u->GetYaxis()->SetRangeUser(-3.99, 3.99);
    res_b3u->SetTitle(";energy [keV];residuals");
    res_b3u->GetXaxis()->SetNdivisions(509);
    res_b3u->GetXaxis()->SetTitleOffset(3);
    res_b3u->SetTickLength(0.02, "X");
    res_b3u->SetFillColor(kTLigOrange);
    res_b3l->SetFillColor(kTLigOrange);
    res_b2u->SetFillColor(kTLigLightYellow);
    res_b2l->SetFillColor(kTLigLightYellow);
    res_b1u->SetFillColor(kTLigPear);
    res_b1l->SetFillColor(kTLigPear);

    // legend
    TLegend* leg;
    if (dataset==0)
    leg = new TLegend(0.5, 0.62, 0.98, 0.97,
                      Form("#font[82]{M2-enrGe} - %0.1f kg#upointyr", exposure[ds]));
    if (dataset==1)
    leg = new TLegend(0.5, 0.62, 0.98, 0.97,
                      Form("#font[82]{M1-enrBEGe} - %0.1f kg#upointyr", exposure[ds]));
    if (dataset==2)
    leg = new TLegend(0.5, 0.62, 0.98, 0.97,
                      Form("#font[82]{M1-enrCoax} - %0.1f kg#upointyr", exposure[ds]));
    leg->SetFillStyle(0);
    leg->SetNColumns(4);
    leg->AddEntry(data, "Data", "F");
    auto tmp = dynamic_cast<TH1D*>( total->Clone() );
    tmp->SetLineWidth(4);
    leg->AddEntry(tmp, "Model", "L");
    for (auto& h: comp) {
        tmp = dynamic_cast<TH1D*>(h->Clone());
        tmp->SetLineWidth(4);
        if(dataset==0 && (string)h->GetName()=="alpha") continue; // no alphas in M2 spectra
        leg->AddEntry(tmp, nice_names[h->GetName()].c_str(), "L");
    }
    leg->SetMargin(0.35);

    // graphics settings
    auto canvas = new TCanvas(Form("amodel_canvas_%s", ds.c_str()), "canvas for a-model results");
    auto dataPad = new TPad("dataPad", "pad for the components", 0.0, 0.3, 1.0, 1.0);
    auto resPad = new TPad("resPad", "pad for the residuals", 0.0, 0.0, 1.0, 0.3);
    dataPad->SetBottomMargin(0.01);
    dataPad->SetLogy();
    resPad->SetTopMargin(0.01);
    resPad->SetBottomMargin(0.35);
    resPad->SetTicks(1,1);
//    resPad->SetGridx();

    // GERDA watermark
    auto gerdawtr = new TLatex(0.992, 0.7, "GERDA 19-09");
    gerdawtr->SetNDC();
    gerdawtr->SetTextFont(43);
    gerdawtr->SetTextSizePixels(16);
    gerdawtr->SetTextAngle(90);

    // ROI box
    int low_edge = data->GetXaxis()->GetBinLowEdge(data->GetXaxis()->FindBin(2014));
    int up_edge = data->GetXaxis()->GetBinLowEdge(data->GetXaxis()->FindBin(2064));
    auto box = new TBox(low_edge, data->GetMinimum(), up_edge, data->GetMaximum()-300);
    box->SetFillColorAlpha(kGray, 1); // this not to cover the overlaying histograms
    box->SetFillStyle(1001);
    box->SetLineWidth(0);
    auto line = new TLine(2039, data->GetMinimum(), 2039, data->GetMaximum());

    auto box_res = dynamic_cast<TBox*>(box->Clone());
    box_res->SetFillColorAlpha(kGray, 1); // this not to cover the overlaying histograms

    // Draw!
    canvas->cd();
    dataPad->Draw();
    dataPad->cd();
    data->Draw();
    if(dataset!=0) box->Draw(); // no blinding in M2 data set
    for (auto& h : comp) h->Draw("SAME HIST");
    total->Draw("SAME HIST");
    leg->Draw();
    gerdawtr->Draw();
    dataPad->RedrawAxis("");
    dataPad->RedrawAxis("G");

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
    if(dataset!=0) box_res->DrawBox(low_edge, -4, up_edge, 4); // no blinding in M2 data set
    resPad->RedrawAxis("");
    resPad->RedrawAxis("G");

    canvas->Update();
    canvas->SaveAs(Form("gmodel-%s.pdf" , ds.c_str()));

    return;
}

struct BIstruct {
    std::string name;
    double total;
    std::vector<double> parts;
};

bool custom_comp(
        const BIstruct& a,
        const BIstruct& b) {
    return a.total > b.total;
}

void plot_BI(int dataset) {

    auto ds = datdict[dataset];
    auto f = new TFile("gmodel-analysis.root");
    TTreeReader reader("BI_pdfs_result", f);
//    TTreeReaderValue<double> marg_mode(reader, "BI_marginalized_mode");
    TTreeReaderValue<double> marg_mode(reader, "BI_best_fit");
    TTreeReaderValue<int> channel(reader, "channel");
    TTreeReaderArray<char> name(reader, "name");

    std::vector<BIstruct> comp;

    // fill data structures
    for (auto& n : cnames) {
        BIstruct bi;
        bi.name = n.first;
        bi.total = 0;

        // std::cout << n.first << std::endl;
        for (auto& p : n.second) {
            reader.SetEntry(p);
            if (*channel == dataset) {
                // std::cout << &name[0] << " --> " << *marg_mode << std::endl;
                bi.parts.push_back(*marg_mode);
                bi.total += *marg_mode;
            }
        }
        comp.push_back(bi);
    }

    // sort stuff
    std::sort(comp.begin(), comp.end(), custom_comp);
    for (auto& c : comp) std::sort(c.parts.begin(), c.parts.end(), std::greater<double>());

    // alphas hack to add offset and slope
    for (auto& c : comp) {
        if(c.name == "alpha") {
            if(c.parts.size()<=2) break;
            c.parts.at(0) += c.parts.at(2);
            c.parts.pop_back();
            break;
        }
    }

    // delete component if BI is too small
    for (auto it = comp.begin(); it != comp.end(); ) {
        if (it->total < 1E-3) it = comp.erase(it);
        else {
            for (auto itp = it->parts.begin(); itp != it->parts.end(); ) {
                if (*itp < 0.1*it->total) itp = it->parts.erase(itp);
                else ++itp;
            }
            ++it;
        }
    }

    // for (auto& c : comp) std::cout << c.name << " "; std::cout << std::endl;

    // hack to join 2nbb and 228Ac (the two smallest)
    if (dataset != 0) {
        comp[comp.size()-2].total += comp[comp.size()-1].total;
        comp[comp.size()-2].parts = std::vector<double>{};
        comp.pop_back();
    }

    double totalBI = 0;
    for (auto& c : comp) totalBI += c.total;

    TCanvas c("", "", 600, 100);

    double start = 0;
    double lr_frac = 0.002;
    double td_frac = 0.02;
    std::vector<std::pair<TBox, std::vector<TBox>>> boxes;
    for (auto& c : comp) {
        auto b_begin = start*(1-2*lr_frac)/totalBI + lr_frac;
        auto b_end = (start+c.total)*(1-2*lr_frac)/totalBI + lr_frac;

        TBox b(b_begin, 0 + td_frac, b_end, 1-td_frac);
        b.SetFillColorAlpha(hcolor[c.name], 0.8);
        // b.SetLineColorAlpha(kBlack, 0.6);
        b.SetLineWidth(2);

        std::vector<TBox> sboxes;
        if (c.total > 5E-01) {
            double sstart = 0;
            for (auto& p : c.parts) {
                TBox sb(b_begin + sstart*(b_end-b_begin)/c.total, 0 + td_frac,
                        b_begin + (sstart + p)*(b_end-b_begin)/c.total, 1-td_frac);
                sb.SetFillColorAlpha(0, 0);
                sboxes.push_back(sb);
                sstart += p;
            }
        }

        boxes.push_back({b, sboxes});
        start += c.total;
    }

    for (auto& b : boxes) for (auto& sb : b.second) sb.Draw("L");
    for (auto& b : boxes) b.first.Draw("L");

    c.SaveAs(("BI-boxes-" + ds + ".pdf").c_str());
}

void gmodel_plot() {

    tolcols::init();
    gROOT->Macro("../../../../../../tools/gerda-style-long.C");

    gROOT->ForceStyle();
    gROOT->SetBatch();
    plot_gmodel(0, 560, 3490, 0.02, 2.0E3, 5);
    plot_gmodel(1);
    plot_gmodel(2);

    plot_BI(0);
    plot_BI(1);
    plot_BI(2);
}

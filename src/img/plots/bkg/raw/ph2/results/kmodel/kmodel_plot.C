/* kmodel_plot_1d.C
 *
 * Author: Luigi Pertoldi
 * Created: 30 Jun 2018
 *
 */

#include "kmodel_common.hpp"

void plot_histos(TH1D* data_raw,
                 TH1D* total_raw,
                 std::vector<TH1D*> comp,
                 int ds,
                 double yrangelow,
                 double yrangeup,
                 std::string do_not_join) {

    auto remove_bin = [&](TH1D* h) {
        auto h_r = new TH1D(h->GetName(), h->GetTitle(), h->GetNbinsX()-1, 0, h->GetNbinsX()-1);
        for (int i = 1; i <= h->GetNbinsX()-1; ++i) {
            if (i < 7) h_r->SetBinContent(i, h->GetBinContent(i));
            else       h_r->SetBinContent(i, h->GetBinContent(i+1));
        }
        return h_r;
    };

    // put together same isotope contributions
    std::vector<TH1D*> comp_sum_raw;
    auto h_tmp = dynamic_cast<TH1D*>(data_raw->Clone());
    for (auto& i : inames) {
        if (i == do_not_join) {
            for (auto& h : comp) {
                std::string s(h->GetName());
                if (s.find(i) != std::string::npos) {
                    // anyways, put together exact copies
                    if (s.find("Copy") != std::string::npos) {
                        auto id = find_if(comp_sum_raw.begin(), comp_sum_raw.end(),
                                          [&](TH1D* hr) {
                                              auto sr = s;
                                              return std::string(hr->GetName()) == sr.erase(sr.find("Copy"));
                                          }
                                  );
                        if (id == comp_sum_raw.end()) {
                            std::cout << "Original copy of " << s << " not found!\n";
                            return 1;
                        }
                        else {
                            (*id)->Add(dynamic_cast<TH1D*>(h->Clone()));
                            continue;
                        }
                    }
                    comp_sum_raw.push_back(dynamic_cast<TH1D*>(h->Clone()));
                    s.erase(s.end()-2, s.end());
                    comp_sum_raw.back()->SetName(s.c_str());
                    //std::cout << comp_sum_raw.back()->GetName() << std::endl;
                }
            }
            continue;
        }
        h_tmp->Reset();
        bool found = false;
        for (auto& h : comp) {
            if (std::string(h->GetName()).find(i) != std::string::npos) {
                h_tmp->Add(h);
                found = true;
            }
        }
        if (found) {
            h_tmp->SetName(i.c_str());
            comp_sum_raw.push_back(dynamic_cast<TH1D*>(h_tmp->Clone()));
        }
    }

    // now merge components with dictionary, if there
    for (auto& m : cmerge) {
        h_tmp->Reset();
        bool found = false;
        for (auto& c : m.second) {
            auto id = find_if(comp_sum_raw.begin(), comp_sum_raw.end(),
                              [&](TH1D* hr) { return std::string(hr->GetName()) == c; }
                      );
            if (id == comp_sum_raw.end()) {
                continue;
            }
            else {
                found = true;
                h_tmp->Add(dynamic_cast<TH1D*>((*id)->Clone()));
                comp_sum_raw.erase(id);
            }
        }
        if (found) {
            comp_sum_raw.push_back(dynamic_cast<TH1D*>(h_tmp->Clone()));
            comp_sum_raw.back()->SetName((m.first).c_str());
        }
    }

    // eventually remove histograms with zero entries
    for (auto it = comp_sum_raw.begin(); it != comp_sum_raw.end(); it++) {
        if ((*it)->GetEntries() == 0) comp_sum_raw.erase(it);
    }

    // remove GD02D from data, total and each comp_sum
    TH1::AddDirectory(false);
    auto data = remove_bin(data_raw);
    auto total = remove_bin(total_raw);
    std::vector<TH1D*> comp_sum;
    for (auto& h : comp_sum_raw) comp_sum.push_back(remove_bin(h));

    // compute residuals
    auto res     = dynamic_cast<TH1D*>(data->Clone("h_res"));
    auto res_b3u = dynamic_cast<TH1D*>(res->Clone("h_band3_up"));
    auto res_b3l = dynamic_cast<TH1D*>(res_b3u->Clone("h_band3_low"));
    auto res_b2u = dynamic_cast<TH1D*>(res_b3u->Clone("h_band2_up"));
    auto res_b2l = dynamic_cast<TH1D*>(res_b3u->Clone("h_band2_low"));
    auto res_b1u = dynamic_cast<TH1D*>(res_b3u->Clone("h_band1_up"));
    auto res_b1l = dynamic_cast<TH1D*>(res_b3u->Clone("h_band1_low"));

    // for (int b = 1; b <= res->GetNbinsX(); ++b) {
    //     double d = data->GetBinContent(b);
    //     double m = total->GetBinContent(b);
    //     double prob = ROOT::Math::poisson_cdf(d, m);
    //     double s;
    //     if (abs(prob) == 1) {
    //         std::cout << "WARNING: poisson_cdf(" << d << ", " << m << ") = 1 in bin " << b << std::endl;
    //         s = prob == 1 ? 3.90 : -3.9;
    //     }
    //     else s = TMath::NormQuantile(prob);

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

    for (int b = 1; b <= res->GetNbinsX(); ++b) {
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

    /* ---------------------------------------------------------------------- */
    // set histogram properties
    data->SetTitle(";;counts / detector");
    data->SetLabelSize(0, "X");
    data->SetMarkerStyle(20);
    data->GetYaxis()->SetRangeUser(yrangelow, yrangeup);
    data->GetXaxis()->SetRangeUser(0, 36); // no naturals
    data->GetXaxis()->SetNdivisions(res->GetNbinsX(), 0, 0);

    for (auto& h : comp_sum) {
        h->SetLineColor(hcolor[h->GetName()]);
        h->SetFillColorAlpha(hcolor[h->GetName()], 0.1);
        h->SetFillStyle(1001);
    }

    res->SetMarkerStyle(20);

    for (auto& h : {res, res_b3u, res_b3l, res_b2u, res_b2l, res_b1u, res_b1l}) {
        h->SetLineWidth(0);
        h->GetXaxis()->SetNdivisions(res->GetNbinsX(), 0, 0);
        h->GetYaxis()->SetNdivisions(4, 0, 0);
    }
    res_b3u->GetYaxis()->SetRangeUser(-3.99, 3.99);
    res_b3u->GetXaxis()->SetRangeUser(0, 36); // no naturals!
    res_b3u->SetTitle(";;residuals");
    res_b3u->SetTickLength(0.02, "X");
    res_b3u->SetFillColor(kTLigOrange);
    res_b3l->SetFillColor(kTLigOrange);
    res_b2u->SetFillColor(kTLigLightYellow);
    res_b2l->SetFillColor(kTLigLightYellow);
    res_b1u->SetFillColor(kTLigPear);
    res_b1l->SetFillColor(kTLigPear);

    // set x-axis labels
    for (int b = 1; b <= res_b3u->GetNbinsX(); ++b) {
        res_b3u->GetXaxis()->SetBinLabel(b, ("#font[102]{" + det[b-1] + "}").c_str());
    }
    res_b3u->GetXaxis()->LabelsOption("v");
    res_b3u->GetXaxis()->SetLabelOffset(0.015);

    // legend
    TLegend * leg = new TLegend(0.38, 0.73, 0.73, 0.95,
                                Form("%s - 60.2 kg#upointyr", datdict[ds].c_str()));
    leg->AddEntry(data, "Data", "P");
    auto tmp = dynamic_cast<TH1D*>(total->Clone());
    tmp->SetLineWidth(4);
    leg->AddEntry(tmp, "Model", "L");
    for (auto& h: comp_sum) {
        tmp = dynamic_cast<TH1D*>(h->Clone());
        tmp->SetLineWidth(4);
        leg->AddEntry(tmp, nice_names[h->GetName()].c_str(), "L");
    }
    leg->SetBorderSize(0);
    leg->SetNColumns(2);
    leg->SetMargin(0.4);
    leg->SetBorderSize(0);

    // graphics settings
    auto canvas = new TCanvas(Form("kmodel_canvas_%d", ds), "canvas for k-model results");
    auto dataPad = new TPad("dataPad", "pad for the components", 0.0, 0.3, 1.0, 1.0);
    auto resPad = new TPad("resPad", "pad for the residuals", 0.0, 0.0, 1.0, 0.3);

    dataPad->SetBottomMargin(0.01);
    dataPad->SetLogy();
//    dataPad->SetGridx();
    resPad->SetBottomMargin(0.38);
    resPad->SetTopMargin(0.01);
    resPad->SetTicks(1,1);
    resPad->SetGridx();

    // GERDA watermark
    auto gerdawtr = new TLatex(0.99, 0.76, "GERDA 19-09");
    gerdawtr->SetNDC();
    gerdawtr->SetTextFont(43);
    gerdawtr->SetTextSizePixels(19);
    gerdawtr->SetTextAngle(90);

    if (!do_not_join.empty()) {
        canvas->SetCanvasSize(900, 1200);
        gStyle->SetPaperSize(0.01*canvas->GetWw(),
                             0.01*canvas->GetWh());
        dataPad->SetPad(0.0, 0.2, 1.0, 1.0);
        resPad->SetPad(0.0, 0.0, 1.0, 0.2);
        dataPad->SetBottomMargin(0.005);
        gerdawtr->SetY(0.85);
        leg->SetNColumns(3);
        leg->SetX1(0.11);
        leg->SetY1(0.84);
        leg->SetX2(0.9);
        leg->SetY2(0.98);
        data->SetTitleOffset(2.1, "Y");
        res_b3u->SetTitleOffset(2.1, "Y");
    }

    // Draw!
    canvas->cd();
    dataPad->Draw();
    dataPad->cd();
    data->Draw("P");
    total->Draw("SAME HIST");
    for (auto& h : comp_sum) h->Draw("SAME HIST");
    data->Draw("P SAME");
    leg->Draw();
    gerdawtr->Draw();

    canvas->cd();
    resPad->Draw();
    resPad->cd();
    res_b3u->Draw("HIST");
    res_b2u->Draw("SAME HIST");
    res_b1u->Draw("SAME HIST");
    res_b3l->Draw("SAME HIST");
    res_b2l->Draw("SAME HIST");
    res_b1l->Draw("SAME HIST");
    res->Draw("P SAME");
    resPad->RedrawAxis("");
    resPad->RedrawAxis("G");

    canvas->Update();
    if (ds >= 5 and ds <= 9 ) canvas->SaveAs(Form("kmodel-2d-ds%d.pdf", ds));
    else                      canvas->SaveAs(Form("kmodel-1d-ds%d.pdf", ds));

    return;
}

void plot_1d(int dataset = 0,
             double yrangelow = 0.11,
             double yrangeup = 9E2,
             std::string do_not_join = "") {

    if (dataset < 0 or dataset > 9) {
        std::cout << dataset << " : no such dataset!\n";
        return;
    }

    // get histograms
    int ds = dataset;
    auto f = new TFile("kmodel-results.root");
    auto data_raw = dynamic_cast<TH1D*>(f->Get(Form("data_%d", ds)));
    auto total_raw = dynamic_cast<TH1D*>(f->Get(Form("total_%d", ds)));

    // retrieve components
    std::vector<TH1D*> comp;
    for (auto& n : cnames) {
        auto h = dynamic_cast<TH1D*>(f->Get(Form("%s_%d", n.c_str(), ds)));
        if (h) comp.push_back(h);
        h = dynamic_cast<TH1D*>(f->Get(Form("%sCopy_%d", n.c_str(), ds)));
        if (h) comp.push_back(h);
    }

    plot_histos(data_raw, total_raw, comp, dataset, yrangelow, yrangeup, do_not_join);

    return;
}

void plot_2d(int dataset = 0,
             double yrangelow = 0.11,
             double yrangeup = 5E2,
             std::string do_not_join = "") {

    if (dataset < 5 or dataset > 9) {
        std::cout << dataset << " : no such dataset!\n";
        return;
    }

    // get histograms
    int ds = dataset;
    auto f = new TFile("kmodel-results.root");
    auto data_2D  = dynamic_cast<TH2D*>(f->Get(Form("data_%d", ds)));
    auto total_2D = dynamic_cast<TH2D*>(f->Get(Form("total_%d", ds)));

    // retrieve components
    std::vector<TH2D*> comp_2D;
    for (auto& n : cnames) {
        auto h = dynamic_cast<TH2D*>(f->Get(Form("%s_%d", n.c_str(), ds)));
        if (h) comp_2D.push_back(h);
        h = dynamic_cast<TH2D*>(f->Get(Form("%sCopy_%d", n.c_str(), ds)));
        if (h) comp_2D.push_back(h);
    }

    // reset xaxis to 37 bins (no naturals)
    data_2D->GetYaxis()->SetRangeUser(0,37);
    total_2D->GetYaxis()->SetRangeUser(0,37);
    for(auto h : comp_2D) h->GetYaxis()->SetRangeUser(0,37);

    // collapse data, total and components into TH1D histograms
    auto data_raw  = data_2D->ProjectionX(); data_raw->Add(data_2D->ProjectionY());
    auto total_raw = total_2D->ProjectionX(); total_raw->Add(total_2D->ProjectionY());
    std::vector<TH1D*> comp;
    for(auto& h : comp_2D) {
        comp.push_back(h->ProjectionX());
        comp.back()->Add(h->ProjectionY());
        std::string s(comp.back()->GetName());
        s.erase(s.end()-3, s.end());
        comp.back()->SetName(s.c_str());
    }

    plot_histos(data_raw, total_raw, comp, dataset, yrangelow, yrangeup, do_not_join);

    return;
}

void kmodel_plot() {

    tolcols::init();
    gROOT->Macro("gerda-style-kmodel.C");

    gROOT->ForceStyle();
    gROOT->SetBatch();

    plot_1d(0, 0.11, 1.6E3, "K40");
    plot_1d(1, 0.03, 9E3,   "K42");
    plot_1d(2, 0.110, 9E2);
    plot_1d(3, 0.110, 5E2);
    plot_1d(4, 0.110, 9E1);

    plot_2d(5, 0.011, 3E2, "K40");
    plot_2d(6, 0.011, 9E2, "K42");
    plot_2d(7, 0.110, 5E2);
    plot_2d(8, 0.110, 3E2);
    plot_2d(9, 0.110, 2E1);
}

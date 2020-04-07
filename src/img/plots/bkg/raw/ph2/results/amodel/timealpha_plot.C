#include <time.h>
using namespace std;

void setstyle();
TF1 * shift_function_in_time(TF1 * func, struct tm start, struct tm end, int sign);
tm make_tm(int year, int month, int day, int hour, int minute, int second);

void plot_PhII_ds(string ds) {
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    string tf_53 = "%y/%m%F2015-12-20 14:42:53"; /*run53*/
    string tf_54 = "%y/%m%F2016-01-25 16:30:28"; /*run54*/
    //  string tf_95 = "%d/%m/%y%F2018-07-18 14:50:30"; /*run95*/
    //  string tf_96 = "%d/%m/%y%F2018-08-10 13:53:36"; /*run96*/

    struct tm tm_53 = make_tm(2015,12,20,14,42,53); /*run53*/
    struct tm tm_54 = make_tm(2016, 1,25,16,30,28); /*run54*/
    struct tm tm_95 = make_tm(2018, 7,18,14,50,30); /*run95*/
    struct tm tm_96 = make_tm(2018, 8,10,13,53,36); /*run96*/

    Float_t binning = 20; //in days

    // open PhaseII file
    string fn = "timealpha-" + ds + "-results.root";
    TFile * f_data = new TFile(fn.c_str());
    TH1D * data  = (TH1D*)f_data->Get("HTimeAlpha");
    data->SetMarkerStyle(20);
    data->GetYaxis()->SetNdivisions(507);
    data->GetYaxis()->SetRangeUser(0,1.15*data->GetMaximum());
    data->GetXaxis()->SetTimeDisplay(1);
    data->GetXaxis()->SetTimeFormat(tf_54.c_str());
    data->GetXaxis()->SetTitle("date (year / month)");
    data->GetYaxis()->SetTitle("rate (cts / 20day)");

    TH1D * ltf   = (TH1D*)f_data->Get("HLiveTimeFraction");
    TH1D * cdata = (TH1D*)data->Clone("copyHTimeAlpha");
    cdata->Divide(ltf); cdata->Scale(1./binning);
    cdata->GetXaxis()->SetTimeDisplay(1);
    cdata->GetXaxis()->SetTimeFormat(tf_54.c_str());
    cdata->GetXaxis()->SetTitle("date [year / month]");
    cdata->GetYaxis()->SetTitle("corr. rate [cts / day]");
    cdata->SetNdivisions(510,"X");
    cdata->SetNdivisions(505,"Y");
    if (ds == "enrBEGe") cdata->GetYaxis()->SetRangeUser(0.3,1.2*cdata->GetMaximum());
    if (ds == "enrCoax") cdata->GetYaxis()->SetRangeUser(0,1.15*cdata->GetMaximum());

    // get range of data histogram
    unsigned long long maxtime = data->GetXaxis()->GetXmax();

    // open Phase II file
    TF1 * MF  = shift_function_in_time((TF1*)f_data->Get("MF"),     tm_54, tm_54,  0);
    TF1 * MFu = shift_function_in_time((TF1*)f_data->Get("MF_up"),  tm_54, tm_54,  1);
    TF1 * MFl = shift_function_in_time((TF1*)f_data->Get("MF_low"), tm_54, tm_54, -1);

    // draw everything
    TCanvas * can1 = new TCanvas(("raw"+ds).c_str(), "raw data", 1000, 400);
    data->Draw();
    can1->Update();

    // draw livetime fraction in same pad
    Float_t rm = 1.1*ltf->GetMaximum();
    ltf->Scale( gPad->GetUymax()/rm );
    ltf->SetLineColor(kAzure+1);
    ltf->SetMarkerStyle(20);
    ltf->SetMarkerColor(kAzure+1);
    ltf->Draw("Lsame");

    //draw an axis on the right side
    TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),
            gPad->GetUxmax(), gPad->GetUymax(),0,rm,505,"+L");
    axis->SetLineColor(kAzure+1);
    axis->SetTitleColor(kAzure+1);
    axis->SetLabelColor(kAzure+1);
    axis->SetTitle("livetime fraction");
    axis->Draw();

    TCanvas * can2 = new TCanvas(("corr"+ds).c_str(),"corrected data",1000,400);
    cdata->Draw("E1");
    MF->Draw("same"); MFu->Draw("same"); MFl->Draw("same");

    // Legend for PhaseII results
    TTree * t_ab = (TTree*)f_data->Get("time_alpha_results");
    double val, val_e;
    t_ab->SetBranchAddress("best_fit",      &val);
    t_ab->SetBranchAddress("best_fit_error",&val_e);
    t_ab->GetEntry(0); double con_ab = val, con_e_ab = val_e;
    t_ab->GetEntry(1); double amp_ab = val, amp_e_ab = val_e;

    TLegend * l1 = new TLegend(0.42,0.7,0.65,0.97);
    if(ds=="enrBEGe") l1->AddEntry(cdata,"data - ^{enr}BEGe","pl");
    if(ds=="enrCoax") l1->AddEntry(cdata,"data - ^{enr}Coax","pl");
    l1->AddEntry(MF,"f(t) = C + N exp#left(#frac{-ln(2) t}{T_{1/2}}#right)", "l");
    TLegend * l2 = new TLegend(0.65,0.7,0.85,0.97);
    l2->AddEntry(MF,Form("C = (%.1f #pm %.1f) cts / day",con_ab,con_e_ab),"");
    l2->AddEntry(MF,Form("N = (%.1f #pm %.1f) cts / day",amp_ab,amp_e_ab),"");
    l2->AddEntry(MF,"T_{1/2} = (138.4 #pm 0.2) days","");
    l2->Draw();
    l1->Draw();

    // GERDA watermark
    auto gerdawtr = new TLatex(0.975, 0.72, "GERDA 19-09");
    gerdawtr->SetNDC();
    gerdawtr->SetTextFont(43);
    gerdawtr->SetTextSizePixels(15);
    gerdawtr->SetTextAngle(90);
    gerdawtr->Draw();

  // write to disk
  // can1->Print(Form("timealpha-raw-%s.pdf", ds.c_str()));
  can2->Print(Form("timealpha-%s.pdf", ds.c_str()));
}

void timealpha_plot() {
    gROOT->SetBatch();
    gROOT->Macro("gerda-style-timealpha.C");
    gROOT->ForceStyle();

    plot_PhII_ds("enrBEGe");
    plot_PhII_ds("enrCoax");
}

// shift a function in time
TF1 * shift_function_in_time(TF1 * func, struct tm start, struct tm end, int sign) {

  time_t time_start = std::mktime(&start);
  time_t time_end   = std::mktime(&end);
  double delta_s = std::difftime(time_end, time_start);

  int npar = func->GetNpar();
  Double_t * par = func->GetParameters();
  double xmin, xmax; func->GetRange(xmin,xmax);
  double delta_ss = delta_s + (xmax - xmin);

  TF1 * trans;
  // base function
  if(npar == 3) {
    trans = new TF1("f", "[0]+[1]*exp(-(x-[3])*log(2.)/[2])", delta_s, delta_ss); // run53 - run103
    trans->SetParameters(par[0], par[1], par[2], delta_s);
    trans->SetLineStyle(1);
  }
  // upper or lower limit
  else if (npar == 6) {
    if (sign > 0) {
      trans = new TF1("fu", "[0]+[1]*exp(-(x-[6])*log(2.)/[2]) + sqrt( [3]*[3] + pow( exp(-(x-[6])*log(2.)/[2])*[4], 2) + 2*exp(-(x-[6])*log(2.)/[2])*[3]*[4]*[5] )",
                      delta_s, delta_ss);
    }
    else if (sign < 0) {
      trans = new TF1("fu", "[0]+[1]*exp(-(x-[6])*log(2.)/[2]) - sqrt( [3]*[3] + pow( exp(-(x-[6])*log(2.)/[2])*[4], 2) + 2*exp(-(x-[6])*log(2.)/[2])*[3]*[4]*[5] )",
                      delta_s, delta_ss);
    }

    trans->SetParameters(par[0], par[1], par[2], par[3], par[4], par[5], delta_s);
    trans->SetLineStyle(2);
  }
  else {
    throw std::runtime_error("Unexpected number of parameters ");
  }

  trans->SetLineColor(kAzure+1);

  return trans;
}

// fill time struct
tm make_tm(int year, int month, int day, int hour, int minute, int second) {
    std::tm tm = {0};
    tm.tm_year = year - 1900; // years count from 1900
    tm.tm_mon  = month - 1; // months count from January=0
    tm.tm_mday = day; // days count from 1
    tm.tm_hour = hour;
    tm.tm_min  = minute;
    tm.tm_sec  = second;

    return tm;
}

// this sets the GERDA default style for spectra plots
// ("long", 1 panel plots)
{
    cout << "Loading GERDA ROOT-logon...";

    int font     = 43;
    int fontsize = 30;

    // define and load gerda plot style
    TStyle *gerdaStyle  = new TStyle("gerda-style"," GERDA specific ROOT style");

    gerdaStyle->SetColorModelPS(1);
    gerdaStyle->SetLineScalePS(1.4);

    // use plain black on white colors
    gerdaStyle->SetFrameBorderMode(0);
    gerdaStyle->SetCanvasBorderMode(0);
    gerdaStyle->SetPadBorderMode(0);
    gerdaStyle->SetPadColor(0);
    gerdaStyle->SetCanvasColor(0);
    gerdaStyle->SetStatColor(0);
    gerdaStyle->SetPalette(kGreyScale);

    // set the paper & margin sizes
    gerdaStyle->SetPadLeftMargin(0.05);
    gerdaStyle->SetPadRightMargin(0.005);
    gerdaStyle->SetPadBottomMargin(0.25);
    gerdaStyle->SetPadTopMargin(0.011);

    // default canvas size
    gerdaStyle->SetCanvasDefH(600);
    gerdaStyle->SetCanvasDefW(900);
    gerdaStyle->SetPaperSize(0.01*gerdaStyle->GetCanvasDefW(),
                             0.01*gerdaStyle->GetCanvasDefW());

    // default font
    gerdaStyle->SetTextFont(font);
    gerdaStyle->SetTextSize(fontsize);

    // axis labels
    gerdaStyle->SetLabelFont(font, "XY");
    gerdaStyle->SetLabelSize(fontsize, "XY");
    gerdaStyle->SetTitleFont(43, "XY");
    gerdaStyle->SetTitleXSize(fontsize);
    gerdaStyle->SetTitleYSize(fontsize);
    gerdaStyle->SetTitleOffset(3, "X");
    gerdaStyle->SetTitleOffset(0.75, "Y");

    // ticks
    gerdaStyle->SetTickLength(0.01, "Y");

    // grid
    gerdaStyle->SetGridStyle(1);
    gerdaStyle->SetGridColor(kGray);

    // legend
    gerdaStyle->SetLegendFont(font);
    gerdaStyle->SetLegendTextSize(fontsize);
    gerdaStyle->SetLegendBorderSize(0);

    // do not display any of the standard histogram decorations
    gerdaStyle->SetOptStat(false);
    gerdaStyle->SetOptTitle(false);
    gerdaStyle->SetOptFit(0);

    cout << " default style set to \"gerda-style\"\n";
    gROOT->SetStyle("gerda-style");

    /* Use this to draw the GERDA watermark
    auto gerdawtr = new TLatex(0.992, 0.76, "GERDA 18-06");
    gerdawtr->SetNDC();
    gerdawtr->SetTextFont(font);
    gerdawtr->SetTextSizePixels(15);
    gerdawtr->SetTextAngle(90);
    gerdawtr->Draw();
    */

    /* use this to draw the blinding box
    int low_edge = h->GetXaxis()->GetBinLowEdge(h->GetXaxis()->FindBin(2014));
    int up_edge = h->GetXaxis()->GetBinUpEdge(h->GetXaxis()->FindBin(2064));
    auto box = new TBox(low_edge, h->GetMinimum(), up_edge, h->GetMaximum());
    box->SetFillColorAlpha(kBlack, 0.25); // this not to cover the overlaying histograms
    box->SetFillStyle(1001);
    box->SetLineWidth(0);
    auto line = new TLine(2039, dataGe->GetMinimum(), 2039, dataGe->GetMaximum());
    line->SetLineStyle(2);
    box->Draw();
    */
}

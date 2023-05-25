#include<string>

Double_t fitFunc(Double_t *x, Double_t *par){
    Double_t xx = x[0];
    Double_t z = 2 * TMath::Pi() * xx;
    Double_t C = 20.88 * TMath::Power(10, -9);
    Double_t L = par[0];
    Double_t r = par[1];
    Double_t R = par[2];
    Double_t A = 1 - z * z * L * C;
    Double_t B = z * C * r;
    Double_t D = r + (R) * A;
    Double_t E = z * (L + C * (R) * r);

    Double_t val = R * TMath::Sqrt((A*A+B*B)/(D*D+E*E));
    return val;
}
TGraph* trans(TGraph* transfer, TMultiGraph* multi, TCanvas*c, std::string file1, std::string file2, int pos)  {
    ifstream inGen;
    ifstream in;
    Double_t y_max,x_max =0;
    inGen.open(file1);
    in.open(file2);
    double x_res, y_res, x_gen, y_gen;
    y_max = 1;
    x_max = 0;
    while(in.good() && inGen.good()) {
        in >> x_res >> y_res;
        inGen >> x_gen >> y_gen;
        double transferred = y_res / y_gen;
        if (transferred < y_max) {
            y_max = transferred;
            x_max = x_res;
        }
        transfer->SetPoint(transfer->GetN(), (x_res + x_gen) / 2.,transferred);
    }
    in.close();
    inGen.close();

    inGen.open(file1);
    in.open(file2);
    Double_t w1 = 0;
    Double_t d1 = 1;
    Double_t w2 = 0;
    Double_t d2 = 1;

    while(in.good() && inGen.good()) {
        in >> x_res >> y_res;
        inGen >> x_gen >> y_gen;
        double transferred = y_res / y_gen;
        double omega = (x_gen + x_res) /2.;
        double diff = abs((transferred - y_max) - (1. - y_max)/sqrt(2));

        if (diff < d1 && omega < x_max ) {
            d1 = diff;
            w1 = omega;
        } else if (diff < d2 && omega > x_max ) {
            d2 = diff;
            w2 = omega;
        }
    }
    std::cout << "\n\nMin is at:  " << x_max <<  '\n';
    std::cout << "W1: " << w1 << " with difference: " << d1 << '\n';
    std::cout << "Quality from w1: " << w1 / (x_max * ((w1*w1)/(x_max*x_max) - 1.)) << '\n';
    std::cout << "W2: " << w2 << " with difference: " << d2 << '\n';
    std::cout << "Quality from w2: " << w2 / (x_max * ((w2*w2)/(x_max*x_max) - 1.)) << '\n';
    std::cout << "Min of: " << file1 << " is: ( " << x_max << " , " << y_max << ")\n\n";

    in.close();
    c->cd(pos);
    transfer->SetTitle("Transfer function; Frequenza (#Omega); Ampiezza (V)");
    multi->Add(transfer);
    transfer->Draw();
    return transfer;
}

void DrawGraph(TGraph* graph, TCanvas* c, int num, TF1* fit, int R, int color) {
    c->cd(num);
    fit->SetParameters(0.01014, 37.937,R);
    fit->SetLineColor(color);
    graph->Fit("fit", "R");
    graph->SetMarkerStyle(31);
    graph->SetMarkerSize(0.1);
    graph->Draw();
}
/*
 *
 *  AGGREGATE FUNCTION
 *
 *
 */
void aggregate(){
    TCanvas* c = new TCanvas("c1", "Aggregate");
    //c->Divide(3,3);

    TF1* fit = new TF1("fit",fitFunc,1000,30000,3);
    fit->SetParNames("L","r", "R");

    auto multi = new TMultiGraph("mg","mg");
    TGraph* transfer680 = new TGraph();
    TGraph* transfer10k = new TGraph();
    TGraph* transfer100k = new TGraph();

    transfer680->SetLineColor(kBlue);
    transfer10k->SetLineColor(kGreen);
    transfer100k->SetLineColor(kBlack);

    trans(transfer680, multi, c, "680Ohm/FGen - Amp - 680Ohm 1-20KHz.txt","680Ohm/Res - Amp - 680Ohm 1-20KHz.txt", 1);
    trans(transfer10k, multi, c, "10kOhm/Amplitude-FGen.txt","10kOhm/Amplitude-Resistance.txt", 2);
    trans(transfer100k, multi, c, "100kOhm/FGen - Amp - 100kOhm 1-30KHz.txt","100kOhm/Res - Amp- 100kOhm 1-30KHz.txt", 1);

    DrawGraph(transfer680, c, 6, fit, 680, kBlue);
    DrawGraph(transfer10k, c,5,fit, 10000, kGreen);
    DrawGraph(transfer100k, c,4, fit,100000, kBlack);

    c->cd(1);
    multi->SetTitle("Modulo della funzione di trasferimento al variare della resistenza (R);Frequenza (Hz); |H(2#pi#omega)|");
    multi->GetYaxis()->SetTitleOffset(0.5);
    multi->Draw("AP");

    TLegend *leg=new TLegend(.1,.7,.3,.9);
    leg->AddEntry(transfer680, "680Ohm");
    leg->AddEntry(transfer10k, "10kOhm");
    leg->AddEntry(transfer100k, "100kOhm");
    leg->Draw("SAME");
}


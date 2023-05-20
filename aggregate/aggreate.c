#include<string>

Double_t fitFunc(Double_t *x, Double_t *par){
    Double_t xx = x[0];
    Double_t z = 2 * TMath::Pi() * xx;
    /*par[0]=L
    par[1]=C
    par[2]=r
    par[3]=R*/
    Double_t C = 20.88 * TMath::Power(10, -9);
    /* Double_t A = 1 - z * z * par[0] * par[1]; */
    /* Double_t B = z * par[1] * par[2]; */
    /* Double_t D = par[2] + (par[3] + 50) * A; */
    /* Double_t E = z * (par[0] + par[1] * (par[3] + 50) * par[2]); */
    Double_t L = par[0];
    Double_t r = par[1];
    Double_t R = par[2];
    Double_t A = 1 - z * z * L * C;
    Double_t B = z * C * r;
    Double_t D = r + (R + 50) * A;
    Double_t E = z * (L + C * (R + 50) * r);

    Double_t val = R * TMath::Sqrt((A*A+B*B)/(D*D+E*E));
    return val;
}
TGraph* trans(TGraph* transfer, TCanvas*c, std::string file1, std::string file2, int pos)  {
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
    c->cd(pos);
    transfer->SetTitle("Transfer function; Frequenza (#Omega); Ampiezza (V)");
    transfer->Draw();
    return transfer;
}

void aggregate(){
    TCanvas* c = new TCanvas("c1", "Aggregate");
    c->Divide(2,3);

    TF1* fit = new TF1("fit",fitFunc,1000,30000,3);
    fit->SetParNames("L","r", "R");

    TGraph* transfer680 = new TGraph();
    TGraph* transfer10k = new TGraph();
    TGraph* transfer100k = new TGraph();

    transfer680->SetLineColor(kBlue);
    transfer10k->SetLineColor(kGreen);
    transfer100k->SetLineColor(kBlack);

    trans(transfer680, c, "680Ohm/FGen - Amp - 680Ohm 1-20KHz.txt","680Ohm/Res - Amp - 680Ohm 1-20KHz.txt", 1);
    trans(transfer10k, c, "10kOhm/Amplitude-FGen.txt","10kOhm/Amplitude-Resistance.txt", 2);
    trans(transfer100k, c, "100kOhm/FGen - Amp - 100kOhm 1-30KHz.txt","100kOhm/Res - Amp- 100kOhm 1-30KHz.txt", 1);

    fit->SetParameters(0.01014, 37.937,680);
    fit->SetLineColor(kBlue);
    transfer680->Fit("fit", "R");

    fit->SetParameters(0.01014, 37.937,10000);
    fit->SetLineColor(kGreen);
    transfer10k->Fit("fit", "R");

    fit->SetParameters(0.01014, 37.937,100000);
    fit->SetLineColor(kBlack);
    transfer100k->Fit("fit", "R");

    c->cd(6);
    transfer680->SetMarkerStyle(31);
    transfer680->SetMarkerSize(0.1);
    transfer680->Draw();
    c->cd(5);
    transfer10k->SetMarkerStyle(31);
    transfer10k->SetMarkerSize(0.1);
    transfer10k->Draw();
    c->cd(4);
    transfer100k->SetMarkerStyle(31);
    transfer100k->SetMarkerSize(0.1);
    transfer100k->Draw();

    c->cd(3);
    auto multi = new TMultiGraph("mg","mg");
    multi->Add(transfer680);
    multi->Add(transfer10k);
    multi->Add(transfer100k);

    multi->SetTitle("|H(#omega)| al variare della resistenza (R);Pulsazione (#omega); |H(#omega)|");
    multi->GetYaxis()->SetTitleOffset(0.5);
    multi->Draw("AP");

    TLegend *leg=new TLegend(.1,.7,.3,.9);
    leg->AddEntry(transfer680, "680Ohm");
    leg->AddEntry(transfer10k, "10kOhm");
    leg->AddEntry(transfer100k, "100kOhm");
    leg->Draw("SAME");
}

void find_min() {
    ifstream in;
    in.open("680Ohm/FGen - Amp - 680Ohm 1-20KHz.txt");
    double y_max = 0;
    double x_max = 0;
    Float_t x,y;
    while (in.good()) {
        in >> x >> y;
        if (y > y_max && x > 6000) {
            y_max = y;
            x_max = x;
        }
    }
    in.close();

    std::cout << "\nAmplitude of FGen: \n" << "- Maximum: " << "(" << x_max << " , " << y_max << ")";

    in.open("680Ohm/Res - Amp - 680Ohm 1-20KHz.txt");
    double y_min = 10;
    double x_min = 0;
    x= 0;
    y =0;
    while (in.good()) {
        in >> x >> y;
        if (y < y_min) {
            y_min = y;
            x_min = x;
        }
    }
    in.close();

    std::cout << "\nAmplitude of Resistance: \n" << "- Minimum: " << "(" << x_min << " , " << y_min << ")\n";
}


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
    Double_t D = z * (L + C * r * R);

    return  (180. / TMath::Pi()) * TMath::ATan((B * (R*A+r) - A * D)/(A * (R*A+r) + B*D)) + z * par[3] + 0.0021;
}

/*
 *
 *  AGGREGATE FUNCTION
 *
 *
 */
void aggregate(){
    TCanvas* c = new TCanvas("c");
    auto multi = new TMultiGraph("mg","mg");

    TGraph* transfer680 = new TGraph("680Ohm/Res - Phase - 680Ohm 1-20kHz.txt");
    TGraph* transfer10k = new TGraph("10kOhm/Phase-Resistance.txt");
    TGraph* transfer100k = new TGraph("100kOhm/Res - Phase - 100kOhm 1-30KHz.txt");

    TF1* fit = new TF1("fit",fitFunc,1000,30000,4);
    fit->SetParNames("L","r", "R", "q");


    fit->SetParameters(0.01014, 37.937,680, 0.00008);
    transfer680->Fit("fit", "R");

    fit->SetParameters(0.01014, 37.937,10000, 0.00008);
    transfer10k->Fit("fit", "R");

    fit->SetParameters(0.01014, 37.937,100000, 0.00008);
    transfer100k->Fit("fit", "R");

    multi->Add(transfer680);
    multi->Add(transfer10k);
    multi->Add(transfer100k);

    transfer680->SetMarkerStyle(31);
    transfer680->SetMarkerSize(0.2);
    transfer680->SetMarkerColor(kBlue);

    transfer10k->SetMarkerStyle(31);
    transfer10k->SetMarkerSize(0.2);
    transfer10k->SetMarkerColor(kGreen);

    transfer100k->SetMarkerStyle(31);
    transfer100k->SetMarkerSize(0.2);
    transfer100k->SetMarkerColor(kBlack);

    multi->SetTitle("Fase della funzione di trasferimento al variare della resistenza (R);Frequenza (Hz); Fase(H(#omega))");
    multi->GetYaxis()->SetTitleOffset(0.5);
    multi->Draw("AP");

    TLegend *leg=new TLegend(.1,.7,.3,.9);
    leg->AddEntry(transfer680, "680Ohm");
    leg->AddEntry(transfer10k, "10kOhm");
    leg->AddEntry(transfer100k, "100kOhm");
    leg->Draw("SAME");
}


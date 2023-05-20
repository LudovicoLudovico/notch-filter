Double_t fitFunc(Double_t *x, Double_t *par){
    Double_t xx = x[0];
    Double_t z = 2 * TMath::Pi() * xx;
    /*par[0]=L
    par[1]=C
    par[2]=r
    par[3]=R*/
    Double_t A = 1 - z * z * par[0] * par[1];
    Double_t B = z * par[1] * par[2];
    Double_t D = par[2] + (par[3] + 50) * A;
    Double_t E = z * (par[0] + par[1] * (par[3] + 50) * par[2]);

    Double_t val = par[3] / (D * D + E * E) * TMath::Sqrt((A * D + B * E) * (A * D + B * E) + (B * D - A * E) * (B * D - A * E));
    return val;
}
Double_t fitFuncFixedParams(Double_t *x){
    Double_t xx = x[0];
    Double_t z = 2 * TMath::Pi() * xx;
    Double_t L = 0.01014;
    Double_t C = 0.000000022;
    Double_t r =37.937;
    Double_t R = 10000;
    /*par[0]=L
    par[1]=C
    par[2]=r
    par[3]=R*/
    Double_t A = 1 - z * z * L * C;
    Double_t B = z * C * r;
    Double_t D = r + (R + 50) * A;
    Double_t E = z * (L + C * (R + 50) * r);

    Double_t val = R / (D * D + E * E) * TMath::Sqrt((A * D + B * E) * (A * D + B * E) + (B * D - A * E) * (B * D - A * E));
    return val;
}

void sweep10kOhm(){
    TCanvas* c = new TCanvas("c1", "Sweep with 10kOhm");
    c->Divide(2,3);

    TF1* fit = new TF1("fit",fitFunc,0,20000,4);
    fit->SetParameters(0.01014,0.000000022,37.937,10000);
    fit->SetParNames("L","C","r", "R");
    //TF1* fit = new TF1("transfer", "(10000+50)*sqrt(((1E7+38.40-(x*2*pi)^2*(22*10^(-9)*1E7*10.14*10^(-3)))^2+(x*2*pi)^2*(10.14*10^(-3)+22*10^(-9)*1E7*38.40)^2)/((1E7*38.40 + (10000+50)*(1E7+38.40-(x*2*pi)^2*22*10^(-9)*1E7*10.14*10^(-3)))^2+(x*2*pi)^2*(1E7*10.14*10^(-3)+(10000+50)*(10.14*10^(-3)+22*10^(-9)*1E7*38.40))^2))",0, 20000);
    TGraph* ampFGen = new TGraph("Amplitude-FGen.txt");
    c->cd(1);
    ampFGen->Draw();

    TGraph* ampRes = new TGraph("Amplitude-Resistance.txt");
    c->cd(2);
    ampRes->Draw();

    TGraph* phaseFGen = new TGraph("Phase-FGen.txt");
    c->cd(3);
    phaseFGen->Draw();

    TGraph* phaseRes = new TGraph("Phase-Resistance.txt");
    c->cd(4);
    phaseRes->Draw();

    ifstream in;
    in.open("Amplitude-FGen.txt");
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

    in.open("Amplitude-Resistance.txt");
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

    // Transfer function
    TGraphErrors* transfer = new TGraphErrors();
    transfer->SetMarkerStyle(7);
    ifstream inGen;
    inGen.open("Amplitude-FGen.txt");
    in.open("Amplitude-Resistance.txt");
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
        int n = transfer->GetN();
        transfer->SetPoint(n, (x_res + x_gen) / 2.,transferred);
        transfer->SetPointError(n, 0.186, (transferred / y_gen + 1. / y_gen)*0.002);
    }
    in.close();
    std::cout << "Min of transfer function: ( " << x_max << " , " << y_max << " )";
    c->cd(5);
    transfer->SetTitle("Transfer function; Frequenza (#Omega); Ampiezza (V)");
    transfer->Fit("fit", "r");
    transfer->Draw();

    double x_theo_min = 0;
    double y_theo_min = 1;
    for (size_t i = 0; i < 20000000; i++) {
        double j = i;
        double eval =fit->Eval(j/1000.);
        if (eval < y_theo_min) {
            y_theo_min = eval;
            x_theo_min = j;
        }
    }

    std::cout << "THEO Min: " << x_theo_min / 1000. << " , " << y_theo_min;
    c->cd(6);
    TF1* transferFixed = new TF1("transferP", "10000*sqrt()", 0, 20000);
    transferFixed->Draw();
}



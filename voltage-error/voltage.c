#include<fstream>
#include<iostream>

void voltage(){
    TCanvas* c = new TCanvas("c1", "Voltage reading error");
    c->Divide(2,2);

    TGraph* FGen1000 = new TGraph("680KOhm - SquareWave.txt", "%lg %lg %*lg %*lg");
    c->cd(1);
    FGen1000->Draw();

    TGraph* FGen1 = new TGraph("Voltage-Quadra-1Hz-50DutyCicle.txt","%lg %lg %*lg %*lg" );
    c->cd(2);
    FGen1->Draw();

    TH1F* thousand = new TH1F("h1", "Histogarm 1000", 100, -0.2, -0.24);
    ifstream in;
    in.open("Voltage-Quadra-1000Hz-50DutyCicle.txt");
    Float_t x,y,z,w;
    while (in.good()) {
        in >> x >> y >> z >> w;
        if (x > 0.0006) {
            thousand->Fill(y);
        }
    }
    in.close();
    c->cd(3);
    thousand->Fit("gaus");
    thousand->Draw();

    TH1F* one = new TH1F("h2", "Histogarm 1", 100, -0.2415, -0.2385);
    in.open("Voltage-Quadra-1Hz-50DutyCicle.txt");
    while (in.good()) {
        in >> x >> y >> z >> w;
        if (x > 0.0006) {
            one->Fill(y);
        }
    }
    in.close();
    c->cd(4);
    one->Draw();
}


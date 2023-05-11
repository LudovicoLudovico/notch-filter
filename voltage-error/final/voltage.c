#include<fstream>
#include<iostream>

void voltage(){
    TCanvas* c = new TCanvas("c1", "Voltage reading error");
    c->Divide(2,2);

    TGraph* FGen1000 = new TGraph("680Ohm-SquareWave-1000.txt", "%lg %lg %*lg %*lg");
    c->cd(1);
    FGen1000->Draw();

    TGraph* FGen100 = new TGraph("680Ohm-SquareWave.txt","%lg %lg %*lg %*lg" );
    c->cd(2);
    FGen100->Draw();

    Float_t x,y,z,w;
    ifstream in;

    TH1F* thousand = new TH1F("h1", "Histogarm 1000", 100,2.1, 2.5);
    in.open("680Ohm-SquareWave-1000.txt");
    while (in.good()) {
        in >> x >> y >> z >> w;
            std::cout << x << "   "  << y << '\n';
        if (x > 0.00005 && x < 0.0008) {
            thousand->Fill(y);
        }
    }
    in.close();
    c->cd(3);
    thousand->Fit("gaus");
    thousand->Draw();

    TH1F* hundred = new TH1F("h2", "Histogarm 1", 1000, 2.1, 2.5);
    in.open("680Ohm-SquareWave.txt");
    while (in.good()) {
        in >> x >> y >> z >> w;
        if (x > 0.0002)  {
            hundred->Fill(y);
        }
    }
    in.close();
    c->cd(4);
    hundred->Fit("gaus");
    hundred->Draw();
}


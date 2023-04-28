#include<fstream>
#include<iostream>
#include<cmath>

void A10kOhmError() {
    TCanvas* c = new TCanvas("c1", "Error Analysis on Voltage Readings");
    c->Divide(2,1);
    //creazione istogramma
    TH1F *h1 = new TH1F("10kOhm Resistance","Amplitude distribution (10kOhm Resistance)",300,0.482,0.43);
    //lettura da file ascii e riempimento istogramma
    ifstream in;
    in.open("Amplitude-10kOhm-CasualError.txt");
    Float_t x,y;
    while (in.good()) {
        in >> x >> y;
        h1->Fill(y);
    }
    in.close();
    c->cd(1);
    h1->Draw();


    TH1F* h2 = new TH1F("100Ohm Resistance", "Apliude distribution (100Ohm Resistance)", 300, 0.482, 0.43);
    in.open("Amplitude-100Ohm-CasualError.txt");
    while (in.good()) {
        in >> x >> y;
        h2->Fill(y);
    }
    in.close();

    c->cd(2);
    h2->Draw();

    double mean_10 = h1->GetMean();
    double mean_100 = h2->GetMean();
    double rms_10 = h1->GetRMS();
    double rms_100 = h2->GetRMS();
// https://statisticsbyjim.com/hypothesis-testing/f-table/
    std::cout << "F Value: " << pow(pow(rms_100, 2) / pow(rms_10, 2), 2);
}

void sfasamento() {
    TCanvas* c = new TCanvas("c1", "Error Analysis on Phase");
    c->Divide(2,1);
    //creazione istogramma
    TH1F *h1 = new TH1F("10kOhm Resistance","Phase",300,0.482,0.43);
    //lettura da file ascii e riempimento istogramma
    ifstream in;
    in.open("Phase-10kOhm-CasualError.txt");
    Float_t x,y;
    while (in.good()) {
        in >> x >> y;
        h1->Fill(y);
    }
    in.close();
    c->cd(1);
    h1->Draw();

    TH1F *h2 = new TH1F("100 Ohm Resistance","Phase",300,-2,-0.6);
    in.open("Phase-100Ohm-CasualError.txt");
    while (in.good()) {
        in >> x >> y;
        h2->Fill(y);
    }
    in.close();
    c->cd(2);
    h2->Draw();
}

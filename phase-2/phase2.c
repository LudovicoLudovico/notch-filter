void phase2() {
    TCanvas* c = new TCanvas("c1", "Error Analysis on Phase");
    c->Divide(2,1);
    //creazione istogramma
    TH1F *h1 = new TH1F("phase-680.txt","Phase",300,0.482,0.43);
    //lettura da file ascii e riempimento istogramma
    ifstream in;
    in.open("phase-680.txt");
    Float_t x,y;
    while (in.good()) {
        in >> x >> y;
        h1->Fill(y);
    }
    in.close();
    c->cd(1);
    h1->Draw();
}

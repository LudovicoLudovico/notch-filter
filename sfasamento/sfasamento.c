#include<fstream>

void sfasamento() {
    TCanvas* c = new TCanvas();
    c->Divide(2,1);

    TH1F* ch0 = new TH1F("ch0", "Histo ch0", 100, -0.8, -0.4);
    TH1F* ch1 = new TH1F("ch1", "Histo ch1", 100, 0.,0.3);

    ifstream in;
    in.open("ch0.txt");
    Float_t x,y;
    while (in.good()) {
        in >> x >> y;
        ch0->Fill(y);
    }

    in.close();

    in.open("ch1.txt");
    while (in.good()) {
        in >> x >> y;
        ch1->Fill(y);
    }
    in.close();

    c->cd(1);
    ch0->Draw();
    c->cd(2);
    ch1->Draw();
}

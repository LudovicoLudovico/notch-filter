void sweep10kOhm()
{
    TCanvas* c = new TCanvas("c1", "Sweep with 10kOhm");
    c->Divide(2,2);

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
}


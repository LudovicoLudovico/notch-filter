void test(){
auto mg  = new TMultiGraph();

   auto gr1 = new TGraph(); gr1->SetMarkerStyle(20);
   auto gr2 = new TGraph(); gr2->SetMarkerStyle(21);
   auto gr3 = new TGraph(); gr3->SetMarkerStyle(23);
   auto gr4 = new TGraph(); gr4->SetMarkerStyle(24);

   Double_t dx = 6.28/100;
   Double_t x  = -3.14;

   for (int i=0; i<=100; i++) {
      x = x+dx;
      gr1->SetPoint(i,x,2.*TMath::Sin(x));
      gr2->SetPoint(i,x,TMath::Cos(x));
      gr3->SetPoint(i,x,TMath::Cos(x*x));
      gr4->SetPoint(i,x,TMath::Cos(x*x*x));
   }

   mg->Add(gr4,"PL");
   mg->Add(gr3,"PL");
   mg->Add(gr2,"*L");
   mg->Add(gr1,"PL");

   mg->Draw("A pmc plc");
}

void sweep680Ohm(){
    TCanvas* c = new TCanvas("c1", "Sweep with 680Ohm");
    c->Divide(2,3);

    TGraph* ampFGen = new TGraph("FGen - Amp - 680Ohm 1-20KHz.txt");
    c->cd(1);
    ampFGen->Draw();

    TGraph* ampRes = new TGraph("Res - Amp - 680Ohm 1-20KHz.txt");
    c->cd(2);
    ampRes->Draw();

    TGraph* phaseFGen = new TGraph("FGen - Phase - 680Ohm 1-20KHz.txt");
    c->cd(3);
    phaseFGen->Draw();

    TGraph* phaseRes = new TGraph("Res - Phase - 680OHm 1-20KHz.txt");
    c->cd(4);
    phaseRes->Draw();

    ifstream in;
    in.open("FGen - Amp - 680Ohm 1-20KHz.txt");
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

    in.open("Res - Amp - 680Ohm 1-20KHz.txt");
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
    TGraph* transfer = new TGraph();
    ifstream inGen;
    inGen.open("FGen - Amp - 680Ohm 1-20KHz.txt");
    in.open("Res - Amp - 680Ohm 1-20KHz.txt");
    double x_res, y_res, x_gen, y_gen;
    y_max = 1;
    x_max = 0;
    std::cout << "1/SQRT(2) " << 1. / sqrt(2) << '\n';
    while(in.good() && inGen.good()) {
        in >> x_res >> y_res;
        inGen >> x_gen >> y_gen;
        double transferred = y_res / y_gen;
        if (transferred < y_max) {
            y_max = transferred;
            x_max = x_res;
        }
        transfer->SetPoint(transfer->GetN(), (x_res + x_gen) / 2.,transferred);
        if (abs(transferred - (1. / sqrt(2))) < 0.00114) {
            std::cout << transferred - (1. / sqrt(2)) << " ";
            std::cout << "X with height of 1/sqrt(2): " << (x_res + x_gen) / 2. << " and y: " << transferred << "\n";
        }
    }
    in.close();
    std::cout << "Min of transfer function: ( " << x_max << " , " << y_max << " )";
    c->cd(5);
    transfer->SetTitle("Transfer function; Frequenza (#Omega); Ampiezza (V)");
    transfer->Draw();
}


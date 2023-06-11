#include <iostream>
#include <cmath>
#include "light.h"
#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TSystem.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TF2.h"
#include "TStyle.h"
#include "TMultiGraph.h"

int main(){

    // root definitions
    TCanvas c("canvas","turno de FC",1000,800);
    gStyle->SetPalette(kBird);

    // Initialize light-source
    lightsource light;
    light.power = 100;    // watts
    light.pos = {0,0,25.f}; // cm

    // Initialize light-map
    lightmap grid(light, array<int,2>{20,20}, array<float,2>{200.f,200.f}); // cm

    // QUESTIONS
    cout << "\n--------- Célula de 2x3 metros; divisão em células de 10x10cm ---------\n";

    cout << "A potência da fonte é: " << light.power << " Watts\n";

    cout << "A intensidade radiante da fonte é: " << light.power/(4*M_PI) << " Watts\n";

    cout << "A potência média numa célula é: " << grid.CellAverage() << " Watts\n";
    cout << "A potência mediana das células é: " << grid.CellMedian() << " Watts\n";

    vector<cell> sortedCells = grid.CellsSorted();
    int lastSortedIndex = sortedCells.size()-1;
    cout << "A potência mínima das células é: " << sortedCells[0].power << " Watts\n";
    cout << "A potência máxima das células é: " << sortedCells[lastSortedIndex].power << " Watts\n";
    cout << "Quociente of máximo/mediana: " << sortedCells[lastSortedIndex].power/grid.CellMedian() << "\n";
    cout << "Potência total recebida pela placa: " << grid.TotalPower() << " Watts\n";

    // ROOT
    cout << "\n--------- ROOT ---------\n";

    // Histogram - 2D
    TH2F h2("h","Incident power (W); Cells - X (cm); Cells - Y (cm);",grid.GetCellNx(),-grid.GetWid()/2,grid.GetWid()/2,grid.GetCellNy(),-grid.GetHei()/2,grid.GetHei()/2);
    auto cellgrid = grid.GetCells();
    h2.SetStats(0);
    for(auto &a: cellgrid){
        for(auto &b : a){
            h2.Fill(b.pos[0],b.pos[1],b.power);
        }
    }
    h2.SetContour(10000);
    h2.Draw("colz");
    c.Modified();
    c.Update();
    c.SaveAs("incident_power.png","png");
    c.Clear();

    
    // Correlation between cell distance and incident power - small distances
    lightsource light2;
    light2.power = 100;    // watts
    array<float,2> range = {10,1000}; // cm
    const int N = 100;
    float trials[N];
    for(int i = 0; i < N; i++) trials[i] = range[0] + i*(range[1]-range[0])/N;

    float totalPowers[N];
    for(int i = 0; i < N; i++){
        light2.pos = {0,0, trials[i]}; // cm
        lightmap g(light2, array<int,2>{20,20}, array<float,2>{200.f,200.f}); // cm
        totalPowers[i] = g.TotalPower();
    }
    TGraph graph(N,trials,totalPowers);
    graph.SetTitle("Correlation between light distance and incident power;Light source distance (cm);Total incident power (W)");
    graph.Draw("AL");
    c.Modified();
    c.Update();
    c.SaveAs("power_vs_distance.png","png");
    c.Clear();




    // Correlation between cell distance and incident power - big distances
    lightsource light3;
    light3.power = 100;    // watts
    array<float,2> range2 = {1000,1000000}; // cm
    array<float,2> widhei = {200.f,200.f}; // cm
    const int N2 = 10000;
    float trials2[N2];
    for(int i = 0; i < N2; i++) trials2[i] = range2[0] + i*(range2[1]-range2[0])/N;

    float totalPowers2[N2]; // Computational
    float totalPowers3[N2]; // Theoretical
    for(int i = 0; i < N2; i++){
        light3.pos = {0,0, trials2[i]}; // cm
        lightmap g(light3, array<int,2>{20,20}, array<float,2>{widhei[0],widhei[1]}); // cm
        totalPowers2[i] = g.TotalPower();

        // Power_cell = (Light_power/4*pi*r^2)*Panel_area
        totalPowers3[i] = ((widhei[0]*widhei[1])*light3.power) / (4*M_PI*trials2[i]*trials2[i]);
    }

    gPad->SetLogx(); // too many numbers

    TMultiGraph mg("mg","Correlation between light distance and incident power (log);Light source distance (cm);Total incident power (W)");
    TGraph graph2(N2,trials2,totalPowers2);
    TGraph graph3(N2,trials2,totalPowers3);

    graph2.SetLineColor(kGreen+1);
    graph3.SetLineColor(kBlue+2);

    mg.Add(&graph2);
    mg.Add(&graph3);


    mg.Draw("CA");


    c.Modified();
    c.Update();
    c.SaveAs("power_vs_distance_large.png","png");
    c.Clear();

    // CONTOUR LINES - ANALYTICAL
    gPad->SetLogx(0); // reset to normal scale :)

    h2 = TH2F("h","Incident power (W); Cells - X (cm); Cells - Y (cm);",grid.GetCellNx(),-grid.GetWid()/2,grid.GetWid()/2,grid.GetCellNy(),-grid.GetHei()/2,grid.GetHei()/2);
    cellgrid = grid.GetCells();
    h2.SetStats(0);
    for(auto &a: cellgrid){
        for(auto &b : a){
            h2.Fill(b.pos[0],b.pos[1],b.power);
        }
    }
    h2.SetContour(10000);
    h2.Draw("colz");

    TF2 f2("Contour lines","((100/(4*pi))*25)/((25*25+x*x+y*y)^(3/2))",-widhei[0]/2,widhei[0]/2,-widhei[1]/2,widhei[1]/2);
    f2.SetLineColor(kRed-3);
    f2.Draw("same");

    const int numContours = 4;
    double contours[numContours];
    double Emax = sortedCells[lastSortedIndex].power;
    for(int i = 0; i < numContours; i++) contours[i] = (1-i*0.22)*Emax; // (1-i*0.22)/Emax

    for(int i = 0; i < numContours; i++) {
        TGraph g(f2.GetContour(&contours[i]));
        g.SetLineColor(kBlue-2);
        g.SetLineWidth(4);
        g.Draw("same");
    }
    c.Modified();
    c.Update();
    c.SaveAs("incident_power_level_curves.png","png");
    c.Clear();


    // PART A QUESTIONS
    cout << "\n--------- CELLS WITH MOST POWER ---------\n";
    auto CellsDescending = grid.CellsSortedDesc();
    for(int i = 0; i < 10; i++) cout << "Cell number " << i << " has coordinates (" << CellsDescending[i].pos[0] << " , " << CellsDescending[i].pos[1] << " , " << CellsDescending[i].pos[2] << " )cm with power " << CellsDescending[i].power << " Watts\n";


    // ERROR
    lightmap err1(light, array<int,2>{21,21}, array<float,2>{200.f,200.f});
    lightmap err2(light, array<int,2>{19,19}, array<float,2>{200.f,200.f});
    cout << "We consider our error to be: " << abs(err1.TotalPower()-err2.TotalPower())/2. << "\n";

    //for(int i = 0; i < )



    cout << "\n";
}

#include <iostream>
#include <array>
#include <math.h>

#include "light.h"
#include "image.h"

#include "TApplication.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TSystem.h"
#include "TH1F.h"
#include "TH2F.h"

using namespace std;

int main(){

    // Initialize light-source
    lightsource light;
    light.power = 100;    // watts
    light.pos = {0,0,-1.f}; // m

    // Initialize light-map
    // NOTA: As contas dão iguais para cm, mm, etc, porque todos os nossos valores são em Watts (as unidades de comprimento cancelam).
    lightmap grid(light, array<int,2>{20,30}, array<float,2>{2.f,3.f}); // metros

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
    TCanvas c("canvas","turno de FC",1000,800);
    cout << "\n--------- ROOT ---------\n";

    // Histogram - 2D
    TH2F h2("h","Incident power (W); Cells - X (m); Cells - Y (m);",grid.GetCellNx(),0,grid.GetWid(),grid.GetCellNy(),0,grid.GetHei());
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
    c.SaveAs("proj2/FIG_LightMap_power.pdf","pdf");
    c.Clear();


    // Convergence graph
    lightsource light2;
    light2.power = 100;    // watts
    light2.pos = {0,0,-.25f}; // m
    array<float,2> dimensions = {2.f,3.f};
    const int N = 7;
    float trials[N] = {.2f, .125f, .0625f, .04f, .025f, .02f, .01f};
    float totalPowers[N];
    for(int i = 0; i < N; i++){
        lightmap g(light2, array<int,2>{int(round(dimensions[0]/trials[i])),int(round(dimensions[1]/trials[i]))}, array<float,2>{dimensions[0],dimensions[1]});
        totalPowers[i] = g.TotalPower();
    }
    TGraph graph(N,trials,totalPowers);
    graph.SetTitle("Correlation between cell size and incident power;Cell side length (m);Total incident power (W)");
    graph.GetYaxis()->SetTitleOffset(1.6);
    graph.Draw("AL*");
    c.Modified();
    c.Update();
    c.SaveAs("proj2/FIG_LightMap_totalpower.pdf","pdf");

    cout << "\n";
 
}

#include <iostream>
#include <algorithm>
#include <numeric>
#include "MyFunction.h"
#include "Functor.h"
#include "IntegDeriv.h"
#include "Rwalk1D.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TH1F.h"
#include "TStyle.h"

int main(){
    
    // Integration
    MyFunction func("pow(x,4)*log(x+sqrt(pow(x,2)+1))");
    IntegDeriv analysis(func);
    double IntegralTrapez, ErrorTrapez = 1.E-6, IntegralSimps, ErrorSimps = 1.E-6;
    analysis.TrapezoidalRule(0., 2., IntegralTrapez, ErrorTrapez);
    analysis.SimpsonRule(0., 2., IntegralSimps, ErrorSimps);
    cout << "Trapezoid rule integral:  " << IntegralTrapez << " error: " << ErrorTrapez << ".\n";
    cout << "Simpson integral: " << IntegralSimps << " error: " << ErrorSimps << ".\n";
    func.Draw({0,10},100,{"x","y"});

    // random walk
    int N = 5;
    int steps = 500;

    Rwalk1D walk(N);
    walk.Run(steps);

    double sum = 0;
    for(int i = 1; i <= N; i++){
        auto a = walk.GetTraj(i);
        sum += a[steps] - a[0];
    }
    sum /= N;
    cout << N << " particles " << steps << " steps-> The average distance covered (x_f - x_i) is: " << sum << "\n";

    TCanvas* canvas = new TCanvas("canvas", "Random Walk", 1000, 1000);

    double dt = walk.GetTimeStep();
    double T = dt*steps;
    TGraph *histograms[N];
    for(int i = 0; i < N; i++){
        auto pos = walk.GetTraj(i+1);
        double times[steps];
        for(int j = 0; j < steps; j++) {
            times[j] = j*dt;
        }
        histograms[i] = new TGraph(steps,times,pos.data());
        histograms[i]->SetLineWidth(1.5);
        histograms[i]->SetLineColor(i+1);
        histograms[i]->SetTitle("Random Walk;time [s];position [m]");
    }
    
    histograms[0]->SetName("Random Walk");
    histograms[0]->Draw();
    for(int i = 1; i < N; i++) histograms[i]->Draw("same");
    canvas->Modified();
    canvas->Update();
    canvas->SaveAs("proj4/random_walk.png","png");

}

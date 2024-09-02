#include <iostream>
#include <vector>
#include <functional>
#include <cmath>

#include "ODEpoint.h"
#include "ODEsolver.h"

#include "TCanvas.h"
#include "TGraph.h"

using namespace std;

int main(){

    double g = 9.806; // ms^-2
    double L = 4.0;   // m

    ODEpoint init(0.0, {0.75,0.} ); // coordinates are (angle, angular velocity)

    vector<function<double(ODEpoint)>> F = {
        [](ODEpoint p)->double{ return p.X()[1]; },                 // dAngle = dVelocity*dt
        [g,L](ODEpoint p)->double{ return -(g/L)*sin(p.X()[0]); }   // dVelocity = dAccel*dt = dt*(-g/L)sin(Angle)
    };

    ODEsolver solver(F);
    auto result = solver.LeapFrog(init,0.1,10.);

    double times[result.size()];
    double angles[result.size()];
    double velocities[result.size()];
    for( int i = 0; i < result.size()-1; i++ ) {
        times[i] = result[i].T();
        angles[i] = result[i].X()[0];
        velocities[i] = result[i].X()[1];
    }
    
    TCanvas *C = new TCanvas("","",1000,800);
    TGraph *G = new TGraph(result.size(),times,angles);
    G->SetTitle("Pendulum Motion;time [s];angle [rad]");
    G->Draw("A*");
    C->Modified();
    C->Update();
    C->SaveAs("proj5/pendulum.png","png");
}

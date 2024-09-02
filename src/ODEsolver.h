#ifndef __ODESOLVER__
#define __ODESOLVER__

#include <functional>
#include <iostream>
#include <map>
#include <cmath>

#include "ODEpoint.h"

using namespace std;

// Solves ordinary differential equations
class ODEsolver{
private:
    // vector of functions dictating the behaviour of the ODEPoints
    vector<function<double(ODEpoint)>> F;
    // map of graphs - key(string) -> vector of points (graph)
    map<string, vector<ODEpoint> > MS;
public:
    // constructors
    ODEsolver(const vector< function<double(ODEpoint)> >&);
    ~ODEsolver();
    //set functions
    void SetODEfunc(const vector< function<double(ODEpoint)> >&);
    //solver methods
    const vector<ODEpoint>& Euler(ODEpoint i, double step, double T);
    const vector<ODEpoint>& PredictorCorrector(ODEpoint i, double step, double T);
    const vector<ODEpoint>& LeapFrog(ODEpoint i, double step, double T); // leap frog second order
    const vector<ODEpoint>& RK2(ODEpoint i, double step, double T);
    const vector<ODEpoint>& RK4(ODEpoint i, double step, double T);
};

#endif
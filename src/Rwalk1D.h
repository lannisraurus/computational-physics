#ifndef __RWALK1D__
#define __RWALK1D__

#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <ctime>

using namespace std;

/*
This class implements random walk (brownian motion)!
*/
class Rwalk1D {
private:

    double x0;      // start position.
    int N;          // number of particles.
    double pL, pR;  // movement bias (probability to the left and right).
    double dt, dx;  // time and distance steps.

    // particle trajectories
    map<int, vector<double>> mT;

public:
    // Constructors
    Rwalk1D(int N=1, double x0=0., double pL=0.5, double pR=0.5, double dt=1, double dx=1);
    ~Rwalk1D() = default;

    // Run simulation
    void Run(int nsteps);

    // Getters
    const vector<double>& GetTraj(int n=1);
    double GetTimeStep();
    double GetSpaceStep();

};

#endif
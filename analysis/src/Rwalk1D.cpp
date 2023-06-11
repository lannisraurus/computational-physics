#include "Rwalk1D.h"

Rwalk1D::Rwalk1D(int N, double x0, double pL, double pR, double dt, double dx)
:N(N),
x0(x0),
pL(pL),
pR(pR),
dt(dt),
dx(dx)
{}

void Rwalk1D::Run(int nsteps){

    // needed variables
    auto t = time(0);
    mt19937 R(t);
    uniform_real_distribution<float> D(0,pL+pR);

    // initialize trajectory
    for(int i = 1; i <= N; i++) mT[i] = {x0};

    // calculate trajectories
    for(int i = 0; i < nsteps; i++){
        for(int j = 1; j <= N; j++){
            int mult;
            if(D(R) < pL) mult = -1; else mult = 1;
            mT[j].push_back(mT[j][i]+mult*dx);
        }
    }

}

const vector<double>& Rwalk1D::GetTraj(int n){
    return mT[n];
}

double Rwalk1D::GetTimeStep(){
    return dt;
}

double Rwalk1D::GetSpaceStep(){
    return dx;
}
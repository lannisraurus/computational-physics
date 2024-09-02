#include "ODEsolver.h"

ODEsolver::ODEsolver(const vector< function<double(ODEpoint)> >& functions){
    SetODEfunc(functions);
}

ODEsolver::~ODEsolver(){
    MS.clear();
    F.clear();
}

void ODEsolver::SetODEfunc(const vector< function<double(ODEpoint)> >& functions){
    F = functions;
}

const vector<ODEpoint>& ODEsolver::Euler(ODEpoint i, double step, double T){
    // initialize MS entry
    MS["euler"].clear();
    MS["euler"].push_back(i);
    // Euler method
    for(double j = i.T()+step; j < T; j+=step){
        ODEpoint cur = MS["euler"].back();
        vector<double> funcs;
        for(auto &f: F) funcs.push_back(f(cur));
        ODEpoint next(cur.T()+step , step*Xvar(funcs)+cur);
        MS["euler"].push_back(next); 
    }
    return MS["euler"];
}



const vector<ODEpoint>& ODEsolver::PredictorCorrector(ODEpoint i, double step, double T){
    // initialize MS entry
    MS["trapezoidal"].clear();
    MS["trapezoidal"].push_back(i);
    // Trapezoidal method
    for(double j = i.T()+step; j < T; j+=step){
        ODEpoint cur = MS["trapezoidal"].back();
        vector<double> nextVars(cur.X().size());
        nextVars.back() = cur.X().back() + step*F.back()(cur);
        for(int i = nextVars.size()-2; i >= 0; i--){
            nextVars[i] = cur.X()[i] + step*(cur.X()[i+1]+nextVars[i+1])/2;
        }
        ODEpoint next(cur.T()+step , nextVars );
        MS["trapezoidal"].push_back(next); 
    }
    return MS["trapezoidal"];
}



const vector<ODEpoint>& ODEsolver::LeapFrog(ODEpoint i, double step, double T){
    // initialize MS entry
    MS["leapfrog"].clear();
    MS["leapfrog"].push_back(i);
    // leap frog method - kickstart process
    double X0 = MS["leapfrog"].back().X()[0];
    double V0 = MS["leapfrog"].back().X()[1];
    MS["leapfrog"].push_back(ODEpoint(MS["leapfrog"].back().T()+step , { X0-step*V0+step*step*0.5*F[1](MS["leapfrog"].back()) , NULL } ));
    // leap frog - continue
    for(double j = i.T()+step; j < T; j+=step){
        ODEpoint cur = MS["leapfrog"].back();
        double Xcur = cur.X()[0];
        double Vcur = cur.X()[1];
        double Xprev = MS["leapfrog"][MS["leapfrog"].size()-2].X()[0];
        ODEpoint next(cur.T()+step , { 2*Xcur - Xprev + step*step*F[1](cur) , NULL} );
        MS["leapfrog"].back()[1] = (next.X()[0]-Xprev)/(2.*step); // update previous velocity
        MS["leapfrog"].push_back(next);
    }
    MS["leapfrog"].pop_back(); // remove last point - out of bounds and no velocity defined.
    return MS["leapfrog"];
}


/*
const vector<ODEpoint>& ODEsolver::RK2(ODEpoint i, double step, double T){
    // initialize MS entry
    MS["RK2"].clear();
    MS["RK2"].push_back(i);
    for(double j = i.T()+step; j < T; j+=step){
        ODEpoint cur = MS["RK2"].back();
        double vMid = cur.X()[1] + 0.5*step*F[1](cur);
        double xNext = cur.X()[0] + step*vMid;
        // double vNext = (xNext-cur.X()[0])/dt;
        double vNext = cur.X()[1] + step*F[1](ODEpoint(0,{xNext,0}));
        ODEpoint next(cur.T()+step, { xNext , vNext } );
        MS["RK2"].push_back(next);
    }
    return MS["RK2"];
}


const vector<ODEpoint>& ODEsolver::RK2(ODEpoint i, double step, double T){
    // initialize MS entry
    MS["RK2"].clear();
    MS["RK2"].push_back(i);
    for(double j = i.T()+step; j < T; j+=step){
        ODEpoint cur = MS["RK2"].back();
        


        ODEpoint next(cur.T()+step, {} );
        MS["RK2"].push_back(next);
    }
    return MS["RK2"];
}



const vector<ODEpoint>& ODEsolver::RK4(ODEpoint i, double step, double T){
    MS["RK4"].clear();
    MS["RK4"].push_back(i);
    for(double j = i.T()+step; j < T; j+=step){
        ODEpoint cur = MS["RK2"].back();
        double K1 = step*cur.X()[1];
        ODEpoint midk2(cur.T()+step/2. , {cur.X()[0]+K1/2. , NULL} );
        midk2 = ODEpoint(midk2.T() , {midk2.X()[0], cur.X()[1] +0.5*step*F[1](midk2)}); //  ( cur.X()[1] + 0.5*dt*F[1](ODEpoint(cur.T()+dt/2. , {cur.X()[0]+K1/2. , NULL})) )
        double K2 = step*midk2.X()[1];
        double K3;
        double K4;
        double xNext = cur.X()[0] + (1/6.)*(K1+2*K2+2*K3+K4);
        ODEpoint next(cur.T()+step, { xNext , (xNext-cur.X()[0])/step} );
        MS["RK2"].push_back(next);
    }
    return MS["RK2"];
}

*/


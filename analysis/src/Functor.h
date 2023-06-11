
#ifndef __FUNCTOR__
#define __FUNCTOR__

#include <iostream>

#include "TCanvas.h"
#include "TApplication.h"
#include "TSystem.h"
#include "TGraph.h"
#include "TAxis.h"


using namespace std;

class Functor{

protected:
    static TCanvas *c;
    string name;

public:
    
    Functor(string name_="");
    virtual ~Functor() = default;

    virtual double operator() (double& x);
    virtual void Draw(array<float,2> range, int npoints, array<string,2> titles);
    
};

#endif

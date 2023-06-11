#ifndef __MYFUNCTION__
#define __MYFUNCTION__

#include "Functor.h"
#include <cmath>
#include "TF1.h"



class MyFunction: public Functor {
public:
    MyFunction(string name_);
    ~MyFunction() = default;
    double operator() (double& x);
};

#endif
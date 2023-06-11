
#ifndef __INTEGDERIV__
#define __INTEGDERIV__

#include "Functor.h"

class IntegDeriv {
 public:
    IntegDeriv(Functor&);
    ~IntegDeriv() = default;

    // integration methods
    
    void TrapezoidalRule(double xi, double xf, double& Integral, double& Error);
    void SimpsonRule(double xi, double xf, double& Integral, double& Error);

    // derivation methods

    double ForwardDeriv(double x, double dist);
    double BackwardDeriv(double x, double dist);
    double CenterDeriv(double x, double dist);

    double Deriv2(double xi, double xf);
    double Deriv4(double xi, double xf);


private:
    
    Functor& F;

};

#endif

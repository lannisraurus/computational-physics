#include "IntegDeriv.h"
#include <cmath>

IntegDeriv::IntegDeriv(Functor& f)
: F(f)
{}

//Writting the derivative methods

double IntegDeriv::BackwardDeriv(double x, double dist) {
    double p1 = x;
    double p2 = x-dist;
    return (F(p1)-F(p2))/dist;
}

double IntegDeriv::ForwardDeriv(double x, double dist) {
    double p1 = x+dist;
    double p2 = x;
    return (F(p1)-F(p2))/dist;
}

double IntegDeriv::CenterDeriv(double x, double dist) {
    double p1 = x+dist;
    double p2 = x-dist;
    return (F(p1)-F(p2))/(2.*dist);
}


double IntegDeriv::Deriv2 (double xi, double xf){   
    double div = (xf-xi)/2.;
    double pm = xi+div;
    double p1 = xi;
    double p2 = xf;
    return (F(p1)-2.*F(pm)+F(p2))/(div*div);
}

double IntegDeriv::Deriv4 (double xi, double xf){
    double div = (xf-xi)/2.;
    double pm = xi+div;
    double p1 = xi;
    double p2 = xf;
    return (F(p1)+F(p2)-2*F(pm))/(div*div*div*div) - (Deriv2(xi,xf))*12./(div*div);
    //Formula obtained in the same way that the 2nd derivative was
}

//Writing the integration methods

void IntegDeriv::TrapezoidalRule(double xi, double xf, double& Integral, double& Error) {

    //Integral = 
    double error = 999;
    int n = 0;
    double div;
    double integral;
    double k1,k2;
    while (error > Error) {
        n++;
        div = (xf-xi)/n;
        integral = 0.;
        error = 0.;
        for (int i = 1; i <= n; i++){
            k1 = xi+div*double(i-1);
            k2 = xi+div*double(i);
            integral += div/2.*(F(k1)+F(k2));
            error += abs(((-div*div*div)/12.)*Deriv2(k1,k2));
        }
    }
    Integral = integral;
    Error = error;
}

void IntegDeriv::SimpsonRule(double xi, double xf, double& Integral, double& Error) {
   
    double error = 999;
    int n = 0;
    double div;
    double integral;
    double k1,k2,k3;
    while (error > Error) {
        n += 2;
        div = (xf-xi)/n;
        integral = 0.;
        error = 0.;
        for (int i = 1; i <= n; i+=2){
            k1 = xi+div*double(i-1);
            k2 = xi+div*double(i);
            k3 = xi+div*double(i+1);
            integral += div/3.*(F(k1)+4.*F(k2)+F(k3));
            error += abs((div*div*div*div*div)/180.*Deriv4(k1,k3));
        }
    }
    Integral = integral;
    Error = error;   
}
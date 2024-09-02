#include "ODEpoint.h"

// Xvar methods

Xvar::Xvar(int n){
    x = vector<double>(n);
}

Xvar::Xvar(vector<double> v){
    x = v;
}

Xvar::Xvar(const initializer_list<double>& v){
    x.insert(x.begin(),v.begin(),v.end());
}

Xvar::Xvar(const Xvar& var){
    x = var.x;
}

Xvar::~Xvar(){
    x.clear();
}

Xvar& Xvar::operator=(const Xvar& copy){
    x = copy.x;
    return *this;
}

Xvar Xvar::operator+(const Xvar& add){
    Xvar result;
    for (int i = 0; i < add.x.size(); i++) result.x.push_back(add.x[i] + x[i]);
    return result;
}

double& Xvar::operator[](int i){
    return x[i];
}

std::vector<double>& Xvar::X(){
    return x;
}

// Xvar friend methods

Xvar operator*(double k, const Xvar& var){
    Xvar result;
    for (int i =  0; i < var.x.size(); i++) result.x.push_back(k*var.x[i]);
    return result;
}

std::ostream& operator<< (std::ostream& stream, const Xvar& var){
    stream << "variables: (";
    for (int i = 0; i < var.x.size()-1; i++) stream << var.x[i] << " , ";
    stream << var.x.back() << ")\n";
    return stream;
}

//SetODEpoint

void ODEpoint::SetODEpoint(double t_,Xvar& p) {
    t = t_;
    x = p.X();
}

void ODEpoint::SetODEpoint(double t_, const initializer_list<double>& v){
    t = t_;
    x.clear();
    x.insert(x.begin(), v.begin(), v.end());
}

void ODEpoint::SetODEpoint(double t_, vector<double> v){
    t = t_;
    x = v;
}
#include <iostream>
#include <vector>

using namespace std;

#ifndef __XVAR__
#define __XVAR__
// Stores information on variables
class Xvar{
protected:
    // list of all variables
    vector<double> x;
public:
    // constructors
    Xvar() = default;
    Xvar(int); // initialize number of variables
    Xvar(vector<double>); // passing vector
    Xvar(const initializer_list<double>&); // initializer list
    Xvar(const Xvar&); // copy constructor
    ~Xvar();
    // operators
    Xvar& operator=(const Xvar&); // assignment operator
    Xvar operator+(const Xvar&); // operator+
    double& operator[](int); // X[i]
    // Getters
    std::vector<double>& X(); // accessor to x
    // friends - operators
    friend Xvar operator*(double, const Xvar&); // scalar*X
    friend std::ostream& operator<< (std::ostream&, const Xvar&);
};
#endif
Xvar operator* (double, const Xvar&);
std::ostream& operator<< (std::ostream&, const Xvar&);

#ifndef __ODEPOINT__
#define __ODEPOINT__
// Derived class of Xvar - added time variable.
class ODEpoint : public Xvar {
private:
    double t; //time
public:
    // constructors - same as Xvar but with added time!
    ODEpoint(): t(-1) {;}
    ODEpoint(double t_, Xvar a_): t(t_), Xvar(a_) {;}
    ODEpoint(double t_, const vector<double>& v) : t(t_), Xvar(v) {;}
    ODEpoint(double t_, const initializer_list<double>& v) : t(t_), Xvar(v) {;}
    // Setters
    void SetODEpoint(double t_, Xvar& p);
    void SetODEpoint(double t_, const initializer_list<double>& v);
    void SetODEpoint(double t_, vector<double> v);
    // Getters
    double& T() { return t; } // returns time
};
#endif
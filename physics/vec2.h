#pragma once
#include <iostream>
#include <math.h>

// This is a class for 2D vectors. the type is static, meaning you cannot edit the vector itself, only create new ones with operations.
class vec2{
private:
    //vector coordinates
    double x,y;
public:
    vec2(): x(0), y(0) {}
    //initialization
    vec2(double X, double Y): x(X), y(Y) {}
    //dot product
    double operator*(const vec2 v){ return x*v.x + y*v.y; } // (a,b)*(c,d) = a*c+b*d
    //size
    double size(){ return sqrt((*this)*(*this)); } // (1,1).size() = sqrt(2)
    //scale
    vec2 operator*(double k){ double rx = x*k ; double ry = y*k; return vec2(rx,ry); } // k(x,y) = (kx,ky)
    //add vectors
    vec2 operator+(const vec2 v){ return vec2(x+v.x,y+v.y); } // (a,b)+(c,d) = (a+c,b+d)
    //normalize vector
    vec2 normalized(){ double k = (*this).size(); return vec2(x/k,y/k); } // vector becomes normalized (size = 1)
    //print
    void print(){ std::cout << "( " << x << " , " << y << " )\n";}
    //return
    double X(){return x;}
    double Y(){return y;}

};
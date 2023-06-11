#pragma once
#include <iostream>
#include <math.h>

// This is a class for 2D vectors. the type is static, meaning you cannot edit the vector itself, only create new ones with operations.
class vec3{
private:
    //vector coordinates
    double x,y,z;
public:
    vec3(): x(0), y(0), z(0) {}
    //initialization
    vec3(double X, double Y, double Z): x(X), y(Y), z(Z) {}
    //dot product
    double operator*(const vec3& v){ return x*v.x + y*v.y + z*v.z; } // (a,b,c)*(d,e,f) = a*d+b*e+c*f
    //cross product
    vec3 operator%(const vec3& v){ return vec3( y*v.z-v.y*z , v.x*z-x*v.z , x*v.y-v.x*y); } // (a,b,c)x(d,e,f) -> vector perpendicular to both
    //size
    double size(){ return sqrt((*this)*(*this)); } // (1,1,1).size() = sqrt(3)
    //scale
    vec3 operator*(const double &k){ double rx = x*k ; double ry = y*k; double rz = z*k ;return vec3(rx,ry,rz); } // k(x,y,z) = (kx,ky,kz)
    //add vectors
    vec3 operator+(const vec3& v){ return vec3(x+v.x,y+v.y,z+v.z); } // (a,b,c)+(d,e,f) = (a+d,b+e,c+f)
    //subtract vectors
    vec3 operator-(const vec3& v){ return vec3(x-v.x,y-v.y,z-v.z); } // (a,b,c)+(d,e,f) = (a+d,b+e,c+f)
    //normalize vector
    vec3 normalized(){ double k = (*this).size(); return vec3(x/k,y/k,z/k); } // vector becomes normalized (size = 1)
    //print
    void print(){ std::cout << "( " << x << " , " << y << " , " << z << " )\n";}
    //return
    double X(){return x;}
    double Y(){return y;}
    double Z(){return z;}

};
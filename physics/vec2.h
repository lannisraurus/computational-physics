#pragma once
#include <iostream>
#include <math.h>

// This is a class for 2D vectors. the type is static, meaning you cannot edit the vector itself, only create new ones with operations.
class vec2{
private:
    //vector coordinates
    double x,y;
public:
    vec2();
    //initialization
    vec2(double x, double y);
    //dot product
    double operator*(const vec2 v); // (a,b)*(c,d) = a*c+b*d
    //size
    double size(); // (1,1).size() = sqrt(2)
    //scale
    vec2 operator*(double k); // k(x,y) = (kx,ky)
    //add vectors
    vec2 operator+(const vec2 v); // (a,b)+(c,d) = (a+c,b+d)
    //normalize vector
    vec2 normalized(); // vector becomes normalized (size = 1)
    //print
    void print(){ std::cout << "( " << x << " , " << y << " )\n";}
    //what is being different
    bool operator!= (const vec2 v){return (x!=v.x || y!=v.y); }
    bool operator== (const vec2 v){return (x==v.x && y==v.y); }
    //return
    double X(){return x;}
    double Y(){return y;}

};
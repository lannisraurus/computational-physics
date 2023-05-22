#include "vec2.h"

vec2::vec2(){
    x=0;y=0;
};

vec2::vec2(double x, double y){
        this->x = x;
        this->y = y;
    }

double vec2::operator*(const vec2 v){ return x*v.x + y*v.y; } // (a,b)*(c,d) = a*c+b*d

double vec2::size(){ return sqrt((*this)*(*this)); } // (1,1).size() = sqrt(2)

vec2 vec2::operator*(double k){ double rx = x*k ; double ry = y*k; return vec2(rx,ry);} // k(x,y) = (kx,ky)

vec2 vec2::operator+(const vec2 v){ return vec2(x+v.x,y+v.y); } // (a,b)+(c,d) = (a+c,b+d)
    
vec2 vec2::normalized(){ double k = (*this).size(); return vec2(x/k,y/k); } // vector becomes normalized (size = 1)

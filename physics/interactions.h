#pragma once
//C++ LIBRARIES
#include <iostream>
#include <math.h>
#include <vector>
#include <array>
//my libraries
#include "vec2.h"
#include "object.h"
#define G 6.67E-11
#define Ke 8.988E9


//////////////////// FORCES //////////////////// 

void CENTRAL_GRAVITY(object& obj,std::vector<object>& universe){

    for(int i = 0; i < universe.size(); i++){

        vec2 dist = (obj.getPos()*(-1))+universe[i].getPos();
        if(dist.size() > obj["radius"]+universe[i]["radius"]){
            double r = dist.size();
            double m = obj["mass"];
            double M = universe[i]["mass"];
            dist = dist.normalized();
            dist = dist*((G*m*M)/(r*r));

            obj.addForce(dist);
            universe[i].addForce(dist*(-1));

        }
    }
}


void CENTRAL_ELECTRIC(object& obj,std::vector<object>& universe){

    for(int i = 0; i < universe.size(); i++){

        vec2 dist = (obj.getPos()*(-1))+universe[i].getPos();
        if(dist.size() > obj["radius"]+universe[i]["radius"]){
            double r = dist.size();
            double q = obj["charge"];
            double Q = universe[i]["charge"];
            dist = dist.normalized();
            dist = dist*((-1*Ke*q*Q)/(r*r));

            obj.addForce(dist);
            universe[i].addForce(dist*(-1));

        }
    }
}


/*
vec2 CENTRAL_ANTI_GRAVITY(object &object,std::vector<object> universe){
    return CENTRAL_GRAVITY(&object,universe)*(-1);
}


vec2 VISCOSITY(object &object, std::vector<object> universe){
    return object.getVel()*(&object).getProp("viscosity")*(-1);
}

vec2 CENTRAL_SPRING(object &object, std::vector<object> universe){
    vec2 total;
    for(auto &a: universe){
        vec2 dist = (object.getPos()*(-1))+a.getPos();
        if(dist.size()>0){
            double size = (dist.size()-object.getProp("spring_eq"))*object.getProp("spring_constant");
            dist = dist.normalized();
            dist = dist*size;
            total=total+dist;
        }
    }
    return total;
}


vec2 CENTRAL_SPRING_DAMPENED(object &object, std::vector<object> universe){
    vec2 v1 = CENTRAL_SPRING(&object,universe);
    vec2 v2 = VISCOSITY(&object,universe);
    return v1+v2;
}
*/


//////////////////// COLLISIONS //////////////////// 

void BLOCK_COLLISION(object& obj, std::vector<object>& universe){
    vec2 addPos;
    vec2 addVel;
    for(auto &a: universe){
        vec2 dist = obj.getPos()*(-1)+(a.getPos());
        if(dist.size()>0){
            double inside = dist.size()-obj["radius"]-a["radius"];
            if(inside<0){
                dist = dist.normalized();
                addPos = addPos + dist*inside;
                addVel = addVel + dist*(obj.getVel()*dist)*(-1);
                obj.setPos(addPos+obj.getPos());
                obj.setVel(addVel+obj.getVel());
            }
        }
    }
}

void ELASTIC_COLLISION(object& obj, std::vector<object>& universe){
    vec2 addPos(0,0);
    vec2 addVel(0,0);
    for(auto &a: universe){
        vec2 dist = obj.getPos()*(-1)+(a.getPos());
        if(dist.size()>0){
            double inside = dist.size()-obj["radius"]-a["radius"];
            if(inside<0){

                dist = dist.normalized();
                addPos = addPos + dist*inside;
                
                double v1 = dist*a.getVel();
                double v2 = dist*obj.getVel();
                double m1 = a["mass"];
                double m2 = obj["mass"];

                double vf = (2*m1*v1+(m2-m1)*v2)/(m1+m2);

                addVel = addVel + dist*vf + dist*(v2)*(-1);

                obj.setPos(addPos+obj.getPos());
                obj.setVel(addVel+obj.getVel());

            }
        }
    }
}

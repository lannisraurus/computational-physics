#pragma once
//C++ LIBRARIES
#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <array>
//my libraries
#include "vec2.h"


//this is a structure containing information on a physical, mechanical body.
class object{

private:

    vec2 position;     // position vector
    vec2 velocity;     // velocity vector
    vec2 acceleration; // acceleration vector

    vec2 force;        //intermidiary
    vec2 posUpdate;
    vec2 velUpdate;

    std::vector<void (*)(object&,std::vector<object>&)> interactions; // applied interaction functions
    std::map<std::string,double> proprieties;                         // proprieties of the object, the first one ALWAYS being it's mass.

    std::vector<object> connections; // reference to connected object objects

public:

    //intializers
    object(){}
    object(vec2 pos, vec2 vel, vec2 accel, std::map<std::string,double> props):
    position(pos), velocity(vel), acceleration(accel), proprieties(props),
    force(vec2(0,0))
    {}

    //Updaters
    void updateInteractions(double dt, std::vector<object> &universe){
        force = vec2(0,0); posUpdate=position; velUpdate=velocity;     // reset delayed additions
        for(auto &func: interactions) func((*this),universe);          // add all the applied interactions
    }
    void updateStatus(double dt){
        acceleration = force*(1/(*this)["mass"]);
        velocity = velUpdate + acceleration*dt; // dv = da*dt   ->  v+dv = v + da*dt
        position = posUpdate + velocity*dt;     // dx = dv*dt   ->  x+dx = x + dv*dt
    }

    //Getters
    vec2 getPos(){return position;}
    vec2 getVel(){return velocity;}
    vec2 getAcc(){return acceleration; }
    double operator[] (std::string name){ return proprieties[name]; } // returns body propriety
    
    //Adders
    void addInteraction(void (*func)(object&,std::vector<object>&)){ interactions.push_back(func); }
    void addPropriety(std::string name, double value){ proprieties[name] = value; }
    void addForce(vec2 f){force = force + f;}

    //Setters
    void setPos(vec2 pos){posUpdate=pos;}
    void setVel(vec2 vel){velUpdate=vel;}
    void setAcc(vec2 acc){acceleration=acc;}
    
    //Comparators
    bool operator== (const object obj){ return (position==obj.position && velocity==obj.velocity && acceleration==obj.acceleration);}

};



void updateUniverse(std::vector<object>& universe, double dt){
    for(int i = 0; i < universe.size(); i++){ universe[i].updateInteractions(dt,universe); }
    for(int i = 0; i < universe.size(); i++){ universe[i].updateStatus(dt); }
}




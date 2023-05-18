#pragma once
//C++ LIBRARIES
#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <array>
//my libraries
#include "vector.h"


//this is a structure containing information on a physical, mechanical body.
class physics{
private:

    bool hasCollided = false;

    vec2 position; // position vector - where is it?
    vec2 velocity; // velocity vector - how fast is it?
    vec2 acceleration; // acceleration vector - how is the velocity changing?

    vec2 totalForces; // the sum of all total forces
    vec2 totalVelocity; // the sum of all collision stuff
    vec2 totalTp; // the sum of all collision stuff

    std::vector<vec2 (*)(physics,std::vector<physics>)> forces; // total forces - what are the operations i have to check each dt?
    std::vector<std::array<vec2,2> (*)(physics&,std::vector<physics>)> collisions; // total acting collision types
    std::map<std::string,double> proprieties; // proprieties of the object, the first one ALWAYS being it's mass.
public:
    physics();
    //initializer
    physics(vec2 pos, vec2 vel, vec2 accel, std::map<std::string,double> props);
    //updates all the forces for a small dt - EULER'S METHOD - must be called EVERY FRAME
    void updateForces(double dt,std::vector<physics> universe);
    //updates all the remaining physical quantities for a small dt - EULER'S METHOD - must be called EVERY FRAME
    void updateStatus(double dt);
    //Getters
    vec2 getPos();
    vec2 getVel();
    vec2 getAcc();
    std::vector<vec2 (*)(physics,std::vector<physics>)> getForces();
    double getProp(std::string name);
    //Adders
    void addForce(vec2 (*force)(physics,std::vector<physics>));
    void addCollision(std::array<vec2,2> (*func)(physics&,std::vector<physics>));
    void addProp(std::string name, double value);
    //Setters
    void teleport(vec2 pos){position=pos;}
    void setMotion(vec2 vel){velocity=vel;}
    void stop(){position=vec2(0,0);velocity=vec2(0,0);acceleration=vec2(0,0);}
    void collide(){ hasCollided=true; }
    //removers
    void removeForce(vec2 (*force)(physics,std::vector<physics>));


};








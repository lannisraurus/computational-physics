#pragma once
//C++ LIBRARIES
#include <iostream>
#include <math.h>
#include <vector>
#include <map>
#include <set>
#include <array>
#include <SDL2/SDL.h>
//my libraries
#include "vec2.h"
#include "vec3.h"



//this is a structure containing information on a physical, mechanical body.
class object{

private:

    // inherent quantities 

    vec2 position;                              // position vector
    vec2 velocity;                              // velocity vector
    vec2 acceleration;                          // acceleration vector
    std::map<std::string,double> proprieties;   // proprieties of the object, the first one ALWAYS being it's mass.
    std::set<std::string> tags;                 // these are tags - used for classifying objects

    // intermidiary quantities

    vec2 force;        // intermidiary for acceleration
    vec2 posUpdate;    // delayed addition to position
    vec2 velUpdate;    // delayed addition to velocity

    // interaction mediatiors

    std::vector<void (*)(object&,std::vector<object>&)> interactions;      // applied interaction functions

    // other bodies

    std::vector<object*> innerUniverse;                                    // bodies to interact with
    std::vector<object*> connections;                                      // connected bodies for special interactions

public:

    //intializers
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
    double operator[] (std::string name){ return proprieties[name]; }
    std::vector<object*> getConnections(){ return connections; }
    std::vector<object*> getUniverse(){ return innerUniverse; }
    
    //Adders
    void addInteraction(void (*func)(object&,std::vector<object>&)){ interactions.push_back(func); }
    void addPropriety(std::string name, double value){ proprieties[name] = value; }
    void addForce(vec2 f){force = force + f;}
    
    void addConnection(object* obj){ connections.push_back(obj); }
    void addBody(object* obj){ innerUniverse.push_back(obj); }

    //Setters
    void setPos(vec2 pos){posUpdate=pos;}
    void setVel(vec2 vel){velUpdate=vel;}

    //Representations
    void draw(SDL_Renderer* ren){
        SDL_SetRenderDrawColor(ren,(*this)["red"],(*this)["green"],(*this)["blue"],(*this)["alpha"]);
        for(int i = -1; i <= 1; i++){
            for(int j = -1; j <= 1; j++){
                SDL_RenderDrawPoint(ren,position.X()+i,position.Y()+j);
            }
        }
        
        
    }
    void print(){
        std::cout << "Object -> Pos( " << position.X() << " , " << position.Y() << " )" << " Vel( " << velocity.X() << " , " << velocity.Y() << " )\n";
    }
    
};
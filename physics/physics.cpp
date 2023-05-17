#include "physics.h"


physics::physics(){}

physics::physics(vec2 pos, vec2 vel, vec2 accel, std::map<std::string,double> props){
    
        position = pos;
        velocity = vel;
        acceleration = accel;
        totalForces = vec2(0,0);
        proprieties = props;
    }


void physics::updateForces(double dt,std::vector<physics> universe){
        totalForces=vec2(0,0); // reset total force vector
        for(auto &a: forces) totalForces = totalForces + a((*this),universe); // add all the applied forces
        for(auto &a: collisions){
            std::array<vec2,2> col = a((*this),universe);
            velocity = velocity + col[1]; // add all the acting collisions
            position = position + col[0]; // add all acting collision offsets
        }
    }

void physics::updateStatus(double dt){
        acceleration = totalForces*(1/proprieties["mass"]); // a = F/m
        velocity = velocity + acceleration*dt; // dv = da*dt   ->  v+dv = v + da*dt
        position = position + velocity*dt; // dx = dv*dt       ->  x+dx = x + dv*dt
    }
    


vec2 physics::getPos(){return position; }
vec2 physics::getVel(){return velocity; }
vec2 physics::getAcc(){return acceleration; }
std::vector<vec2 (*)(physics,std::vector<physics>)> physics::getForces(){ return forces; }

double physics::getProp(std::string name){return proprieties[name]; }
void physics::addForce(vec2 (*force)(physics,std::vector<physics>)){ forces.push_back(force); }
void physics::addCollision(std::array<vec2,2> (*func)(physics,std::vector<physics>)){ collisions.push_back(func); }
void physics::addProp(std::string name, double value){ proprieties[name] = value; }


void physics::removeForce(vec2 (*force)(physics,std::vector<physics>)){
    std::vector<vec2 (*)(physics,std::vector<physics>)> newForces;
    for(auto &a: forces) if(a!=force) newForces.push_back(a);
    forces = newForces;
}








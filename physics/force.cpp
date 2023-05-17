#include "force.h"

//////////////////// FORCES //////////////////// 

vec2 CENTRAL_GRAVITY(physics object,std::vector<physics> universe){
    vec2 total;
    for(auto &body: universe){
        vec2 dist = (object.getPos()*(-1))+body.getPos();
        if(dist.size() > object.getProp("radius")+body.getProp("radius")){
            double r = dist.size();
            double m = object.getProp("mass");
            double M = body.getProp("mass");
            dist = dist.normalized();
            dist = dist*((G*m*M)/(r*r));
            total=total+dist;
        }
    }
    return total;
}


vec2 CENTRAL_ANTI_GRAVITY(physics object,std::vector<physics> universe){
    return CENTRAL_GRAVITY(object,universe)*(-1);
}


vec2 VISCOSITY(physics object, std::vector<physics> universe){
    return object.getVel()*object.getProp("viscosity")*(-1);
}

vec2 CENTRAL_SPRING(physics object, std::vector<physics> universe){
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


vec2 CENTRAL_SPRING_DAMPENED(physics object, std::vector<physics> universe){
    vec2 v1 = CENTRAL_SPRING(object,universe);
    vec2 v2 = VISCOSITY(object,universe);
    return v1+v2;
}


//////////////////// COLLISIONS //////////////////// 


std::array<vec2,2> SIMPLE_COLLISION(physics object, std::vector<physics> universe){
    vec2 addPos(0,0);
    vec2 addVel(0,0);
    for(auto &a: universe){
        vec2 dist = object.getPos()*(-1)+(a.getPos());
        if(dist.size()>0){
            double inside = dist.size()-object.getProp("radius")-a.getProp("radius");
            if(inside<=0){
                dist = dist.normalized();
                addPos = addPos + dist*inside;
                double proportion = (dist*object.getVel())*(-2);
                addVel = addVel + dist*proportion;
            }
        }
    }
    return {addPos,addVel};
}
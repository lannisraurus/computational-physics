#pragma once
#include "object.h"
#include <SDL2/SDL.h>

///////////   PROPRIETIES

void setPropColour(std::map<std::string,double> &m, int red, int green, int blue, int alpha){
    m["red"]=red;
    m["green"]=green;
    m["blue"]=blue;
    m["alpha"]=alpha;
}
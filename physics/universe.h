#pragma once
#include "object.h"
#include <SDL2/SDL.h>

////////////   UNIVERSE FUNCTIONS


// does both universe updatings in order for the universe
void updateUniverse(std::vector<object>& universe, double dt){
    for(int i = 0; i < universe.size(); i++){ universe[i].updateInteractions(dt,universe); }
    for(int i = 0; i < universe.size(); i++){ universe[i].updateStatus(dt); }
}

// adds an interaction to all members of an universe
void addUniverseInteraction(std::vector<object>& universe, void (*func)(object&,std::vector<object>&)){
    for(auto &a: universe){ a.addInteraction(func); }
}

// adds a member to the universe, updating all members [coming optimization feature]
void addUniverseBody(std::vector<object>& universe, object obj){
    //insert code here
}

void drawUniverse(std::vector<object>& universe, SDL_Renderer* ren){
    for(auto &a: universe) a.draw(ren);
}


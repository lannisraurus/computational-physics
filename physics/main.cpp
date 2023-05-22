#include <iostream>
#include "object.h"
#include "interactions.h"
#include "vec2.h"

#include <stdlib.h>

#include <SDL2/SDL.h>

#define WID 1000
#define HEI 800
#define sim_speed 1

    //add 3d vectors and a vector base class so physics can be either 2d or 3d

int main(){

    SDL_Window *win = nullptr;
    SDL_Event ev;
    SDL_Renderer *ren = nullptr;

    win = SDL_CreateWindow("test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WID,HEI,SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

    std::vector<object> universe;
    std::map<std::string,double> prop;
    prop["mass"] = 1;
    prop["radius"] = 5;
    for(int i = 0; i < 15; i++) for(int j = 0; j < 15; j++){
        if(rand()%2) prop["charge"]=0.01; else prop["charge"]=-0.01;
        universe.push_back(object(vec2(i*50,j*50),vec2(0,0),vec2(0,0),prop));
    }

    for(auto &a: universe) {
        a.addInteraction(&CENTRAL_ELECTRIC);
    }

    bool open = true;
    double t0 = SDL_GetTicks64();
    double dT = 0;
    while(open){
        //events
        while(SDL_PollEvent(&ev)){
            switch(ev.type){
                case SDL_QUIT:
                open=false;
                break;
            }
        }

        //physics
        
        updateUniverse(universe,sim_speed*dT);


        //graphics

        SDL_SetRenderDrawColor(ren,0,0,0,255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren,255,255,255,255);
        for(int i = 0; i < universe.size(); i++) SDL_RenderDrawPoint(ren,universe[i].getPos().X(),universe[i].getPos().Y());
        SDL_RenderPresent(ren);


        //time handler
		dT = ((double)SDL_GetTicks64() - t0) / 1000;
		t0 = (double)SDL_GetTicks64();
    }


    

}
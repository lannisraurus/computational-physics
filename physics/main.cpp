#include <iostream>
#include "physics.h"
#include "force.h"
#include "vector.h"

#include <stdlib.h>

#include <SDL2/SDL.h>

    //add 3d vectors and a vector base class so physics can be either 2d or 3d

int main(){

    SDL_Window *win = nullptr;
    SDL_Event ev;
    SDL_Renderer *ren = nullptr;

    win = SDL_CreateWindow("test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,500,500,SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

    std::vector<physics> universe;

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
        for(auto &a: universe) a.updateForces(dT,universe);
        for(auto &a: universe) a.updateStatus(dT);


        //graphics

        SDL_SetRenderDrawColor(ren,0,0,0,255);
        SDL_RenderClear(ren);
        SDL_SetRenderDrawColor(ren,255,255,255,255);
        for(auto &a: universe) SDL_RenderDrawPoint(ren,a.getPos().X(),a.getPos().Y());
        SDL_RenderPresent(ren);


        //time handler
		dT = ((double)SDL_GetTicks64() - t0) / 1000;
		t0 = (double)SDL_GetTicks64();
    }


    

}
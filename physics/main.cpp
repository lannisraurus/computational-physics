#include <iostream>
#include "physics.h"
#include "force.h"
#include "vector.h"

#include <stdlib.h>

#include <SDL2/SDL.h>

#define WID 1000
#define HEI 800

    //add 3d vectors and a vector base class so physics can be either 2d or 3d

int main(){

    SDL_Window *win = nullptr;
    SDL_Event ev;
    SDL_Renderer *ren = nullptr;

    win = SDL_CreateWindow("test",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,WID,HEI,SDL_WINDOW_SHOWN);
    ren = SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED);

    std::map<std::string,double> q;
    std::map<std::string,double> Q;
    std::vector<physics> universe;
    Q["charge"] = 0.01;
    Q["radius"] = 5;
    Q["mass"] = 300000000000000;
    Q["spring_constant"] = 100000000000;
    Q["spring_eq"] = 200;
    Q["viscosity"] = 2000000000000;
    q=Q;
    q["mass"]=1;

    const int size = 15;
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            universe.push_back(physics(vec2((WID/size)*i,(HEI/size)*j),vec2(-25+rand()%50,-25+rand()%50),vec2(0,0),Q));
            std::cout << "added\n";
        }
    }

    for(auto &a: universe) a.addCollision(&ELASTIC_COLLISION);


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
        
        for(auto &a: universe) a.updateForces(1*dT,universe);
        for(auto &a: universe) a.updateStatus(1*dT);
        


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
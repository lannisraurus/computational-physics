#pragma once
#include <iostream>

// program struct - holds all program variables in a package
struct program{

    // Manager Variables
    file files;

    // SDL Variables
    SDL_Window *window = nullptr;       // Window object
    SDL_Renderer *renderer = nullptr;   // Renderer object
    SDL_Event event;                    // Event object

    // World Variables
    std::vector<object> universe;

    // Program Variables
    bool isOpen;

    // Time Variables
    double t0;
    double dT;

};

// initializes the program variables
void initializeProgram(program &p){
    p.window = SDL_CreateWindow("Raurus Engine",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1920,1080,SDL_WINDOW_SHOWN);
    p.renderer = SDL_CreateRenderer(p.window,-1,SDL_RENDERER_ACCELERATED);
    p.isOpen = !(p.window==nullptr);
    p.t0 = SDL_GetTicks64();
    p.dT = 0;

    std::map<std::string,double> proprieties;
    proprieties["mass"] = 0.1;
    proprieties["radius"] = 1;
    setPropColour(proprieties,255,0,0,255);

    for(int i = 0; i < 100; i++) {
        setPropColour(proprieties,rand()%256,rand()%256,rand()%256,255);
    p.universe.push_back(object(vec2(0,i*10),vec2(100,0),vec2(),proprieties));
    }
    
    
    proprieties["mass"] = 1000000000000000000;
    setPropColour(proprieties,255,255,255,255);
    p.universe.push_back(object(vec2(910,540),vec2(0,50),vec2(),proprieties));
    p.universe.push_back(object(vec2(1600,540),vec2(-0,-50),vec2(),proprieties));

    addUniverseInteraction(p.universe,&CENTRAL_ANTI_GRAVITY);
}

// handles SDL2 events
void handleEvents(program &p){
    while(SDL_PollEvent(&p.event)){
        switch(p.event.type){
            case SDL_QUIT:
            p.isOpen=false;
            break;
        }
    }
}

// handles time variables - tick speed, fps, etc
void handleTime(program &p){
    p.dT = ((double)SDL_GetTicks64() - p.t0) / 1000;
	p.t0 = (double)SDL_GetTicks64();
}

// handles physics
void handlePhysics(program &p){
    updateUniverse(p.universe,p.dT);
}

// handles display
void handleDisplay(program &p){
    SDL_SetRenderDrawColor(p.renderer,0,0,0,255);
    SDL_RenderClear(p.renderer);
    drawUniverse(p.universe,p.renderer);
    SDL_RenderPresent(p.renderer);
}
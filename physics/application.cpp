// C++ Libs
#include <iostream>
#include <stdlib.h>
#include <SDL2/SDL.h>
// Classes
#include "vec2.h"
#include "vec3.h"
#include "file.h"
#include "object.h"
// Functions
#include "universe.h"
#include "interactions.h"
#include "proprieties.h"
#include "handlers.h"

// Main program
int main(){

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) printf("SDL ERROR: %s\n", SDL_GetError());

    // Program Variables
    program p;
    initializeProgram(p);

    // Program Loop
    while(p.isOpen){

        //events
        handleEvents(p);
        //physics
        handlePhysics(p);
        //graphics
        handleDisplay(p);
        //time
		handleTime(p);

    }

    // Close the Program
    SDL_DestroyRenderer(p.renderer);
    SDL_DestroyWindow(p.window);
    return 0;

}
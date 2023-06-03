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
    program program;
    initializeProgram(program);

    // Program Loop
    while(program.isOpen){

        //events
        handleEvents(program);
        //physics
        handlePhysics(program);
        //graphics
        handleDisplay(program);
        //time
		handleTime(program);

    }

    // Close the ProgramPoint2D operator=(const Point2D& p);
    SDL_DestroyRenderer(program.renderer);
    SDL_DestroyWindow(program.window);
    return 0;

}
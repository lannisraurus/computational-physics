#pragma once
//C++ LIBRARIES
#include <iostream>
#include <math.h>
#include <vector>
#include <array>
//my libraries
#include "vector.h"
#include "physics.h"
#define G 0.0000000000667
#define coloumb_const 8988000000


// FORCES

vec2 CENTRAL_ELECTRIC(physics object,std::vector<physics> universe);
vec2 CENTRAL_GRAVITY(physics object,std::vector<physics> universe);
vec2 CENTRAL_ANTI_GRAVITY(physics object,std::vector<physics> universe);
vec2 VISCOSITY(physics object,std::vector<physics> universe);
vec2 CENTRAL_SPRING(physics object, std::vector<physics> universe);
vec2 CENTRAL_SPRING_DAMPENED(physics object, std::vector<physics> universe);

// COLLISIONS

std::array<vec2,2> SIMPLE_COLLISION(physics& object, std::vector<physics> universe);
std::array<vec2,2> ELASTIC_COLLISION(physics& object, std::vector<physics> universe);
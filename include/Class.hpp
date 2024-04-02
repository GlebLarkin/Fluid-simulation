#ifndef FLUID_CLASS_HPP_
#define FLUID_CLASS_HPP_

#include <cmath>

static const double PI = M_PI; //pi 
static const double g = 0.050; //acceleration of free fall
static const unsigned int coef = 1; //the coefficient of proportionality between the repulsive force and the masses of particles divided by the distance between them
static unsigned int boundX = 1200; //size of the sfml window
static unsigned int boundY = 800;
static const float r = 10; //radius of a circle of a particle
static const float Radius_of_Interaction = 1; //the radius of the area of interaction of this particle with the rest
static const float a = 1; //offset 1/x^2 along the x and y axes in the Gradient_of_Interaction method
static const float alpha = 1; //the coefficient of stretching is 1 / x ^ 2 in the Gradient_of_Interaction method

#endif  // FLUID_CLASS_HPP_
#ifndef FLUID_CLASS_HPP_
#define FLUID_CLASS_HPP_

#include <cmath>
#include "nlohmann_json/json.hpp"

struct Data{
    Data(const nlohmann::json& config)
: PI(config["PI"]) ,g(config["g"]), coef(config["coef"]), boundX(config["boundX"]),
  boundY(config["boundY"]), r(config["r"]),
  Radius_of_Interaction(config["Radius_of_Interaction"]), a(config["a"]), alpha(config["alpha"]), Radius_of_Viscosity(config["Radius_of_Viscosity"])
{}
    const double PI, g;
    const unsigned int coef, Radius_of_Interaction, Radius_of_Viscosity;
    unsigned int boundX, boundY;
    const float r, a, alpha;
};
/*

{
  "g": 0.050, 
  "coef": 1,
  "boundX": 1200,
  "boundY": 800,
  "r": 10, 
  "Radius_of_Interaction" : 1,
  "a": 1,
  "alpha": 1

    static const double PI = M_PI; //pi 
    static const double g = 0.050; //acceleration of free fall
    static const unsigned int coef = 1; //the coefficient of proportionality between the repulsive force and the masses of particles divided by the distance between them
    static unsigned int boundX = 1200; //size of the sfml window
    static unsigned int boundY = 800;
    static const float r = 10; //radius of a circle of a particle
    static const float Radius_of_Interaction = 1; //the radius of the area of interaction of this particle with the rest
    static const float a = 1; //offset 1/x^2 along the x and y axes in the Gradient_of_Interaction method
    static const float alpha = 1; //the coefficient of stretching is 1 / x ^ 2 in the Gradient_of_Interaction method
*/
#endif  // FLUID_CLASS_HPP_
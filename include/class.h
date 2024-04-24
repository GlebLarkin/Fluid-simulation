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

#endif  // FLUID_CLASS_HPP_
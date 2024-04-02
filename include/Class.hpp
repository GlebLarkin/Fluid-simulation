#ifndef FLUID_CLASS_HPP_
#define FLUID_CLASS_HPP_

#include <cmath>
/*
#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include <random>
#include <vector>

#include <nlohmann_json/json_fwd.hpp>
#include <nlohmann_json/json.hpp>

#include <fstream>

std::ifstream people_file("people.json", std::ifstream::binary);
nlohmann::Json::Value people;
people_file >> people;

const nlohmann::json& config;
*/

static const double PI = M_PI; //pi 
static const double g = 0.050; //acceleration of free fall
static const unsigned int coef = 1; //the coefficient of proportionality between the repulsive force and the masses of particles divided by the distance between them
static unsigned int boundX = 1200; //size of the sfml window
static unsigned int boundY = 800;
static const float r = 10; //radius of a circle of a particle
static const float Radius_of_Interaction = 1; //the radius of the area of interaction of this particle with the rest
static const float a = 1; //offset 1/x^2 along the x and y axes in the Gradient_of_Interaction method
static const float alpha = 1; //the coefficient of stretching is 1 / x ^ 2 in the Gradient_of_Interaction method

/*
static const double PI = M_PI; //pi 
static const double g = config["g"]; //acceleration of free fall
static const unsigned int coef = config["coef"]; //the coefficient of proportionality between the repulsive force and the masses of particles divided by the distance between them
static unsigned int boundX = config["boundX"]; //size of the sfml window
static unsigned int boundY = config["boundY"];
static const float r = config["r"]; //radius of a circle of a particle
static const float Radius_of_Interaction = config["Radius_of_Interaction"]; //the radius of the area of interaction of this particle with the rest
static const float a = config["a"]; //offset 1/x^2 along the x and y axes in the Gradient_of_Interaction method
static const float alpha = config["alpha"]; //the coefficient of stretching is 1 / x ^ 2 in the Gradient_of_Interaction method
*/

#endif  // FLUID_CLASS_HPP_
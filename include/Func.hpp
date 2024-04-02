#ifndef FLUID_FUNC_HPP_
#define FLUID_FUNC_HPP_

#include <thread>
#include <random>

#include <Particle.hpp>

unsigned int getScreenWidth(); //returns screen size

unsigned int getScreenHeight();

void sleep(int sec);//delay for sec seconds



void left_mouse_click(Particle& A, const sf::RenderWindow* window_ptr);
	//we realize the attraction to the cursor when you click the mouse(lmb)
	//there are two options: depending on the length and on the length squared
	//we choose the second one, because we want to interact more with close particles


void right_mouse_click(Particle& A, const sf::RenderWindow* window_ptr); //we relize repulsion from the cursor when the mouse is clicked(rmb)




double generateRandomNumber(); //generate random number from zero to one


Particle* create_particle_array(const unsigned int number_of_particels); //creates array of particles number_of_particels

#endif  // FLUID_FUNC_HPP_
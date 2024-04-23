#ifndef FLUID_FUNC_HPP_
#define FLUID_FUNC_HPP_

#include <thread>
#include <random>
#include <particle.h>


unsigned int GetScreenWidth(); //returns screen size

unsigned int GetScreenHeight();

void Sleep(int sec);//delay for sec seconds



void LeftMouseClick(Particle& A, const sf::RenderWindow* window_ptr, Data& d);
	//we realize the attraction to the cursor when you click the mouse(lmb)
	//there are two options: depending on the length and on the length squared
	//we choose the second one, because we want to interact more with close particles


void RightMouseClick(Particle& A, const sf::RenderWindow* window_ptr); //we relize repulsion from the cursor when the mouse is clicked(rmb)




double GenerateRandomNumber(); //generate random number from zero to one


Particle* CreateParticleArray(const unsigned int number_of_particels, Data& d, const sf::Texture& texture); //creates array of particles number_of_particels

#endif  // FLUID_FUNC_HPP_
#include "func.h"
#include <memory>

unsigned int GetScreenWidth() //returns screen size
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	return desktop.width;
}

unsigned int GetScreenHeight()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	return desktop.height;
}

void LeftMouseClick(Particle& A, const MouseState& mouse_state, Data& d) {
	//we realize the attraction to the cursor when you click the mouse(lmb)
	//there are two options: depending on the length and on the length squared
	//we choose the second one, because we want to interact more with close particles

	sf::Vector2f direction = sf::Vector2f(mouse_state.position) - A.GetCircle().getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 300) return;

	if ((float)d.boundY - A.GetY() <= 17) { A.SetVy(-0.5); A.SetY((float)d.boundY - 17); return; } //this fixes sticking to the floor

	
	//direction /= (length / 0.4);
	direction = (sf::Vector2f)(direction * 100.0f / (length * length));

	if (length < 40) return; //depending on the square of the length, the particles should not approach the cursor too much

	A.SetVx(A.GetVx() + direction.x);
	A.SetVy(A.GetVy() + direction.y);
}

void RightMouseClick(Particle& A, const MouseState& mouse_state) {
	//we relize repulsion from the cursor when the mouse is clicked(rmb)
	sf::Vector2f direction = sf::Vector2f(mouse_state.position) - A.GetCircle().getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 400) return;
	direction = (sf::Vector2f)(direction * 100.0f / (length * length));

	A.SetVx(A.GetVx() - 0.6 * direction.x);
	A.SetVy(A.GetVy() - 0.6 * direction.y);
}

double GenerateRandomNumber() {
	//generate random number from zero to one
	std::random_device rd;
	std::mt19937 random_number(rd());
	std::uniform_real_distribution<double> dis(0, 1.0);

	return dis(random_number); // We generate and return a random number from 0 to 1
}

Particle* CreateParticleArray(const unsigned int number_of_particels, Data& d, const sf::Texture& texture) {
	//creates array of particles number_of_particels
	Particle * ptr_for_particles_array = static_cast<Particle*>(std::aligned_alloc(alignof(Particle), sizeof(Particle) * number_of_particels));
	
	//Particle* ptr_for_particles_array = new Particle(d) [number_of_particels];

	for (unsigned int i = 0; i < number_of_particels; i++) {
	//Particle* ptr_for_particles_array = new Particle(d) [number_of_particels];
		new (ptr_for_particles_array + i) Particle(d, texture);
		ptr_for_particles_array[i].SetX((float)(d.boundX * GenerateRandomNumber())); //every particle from the array has random coord
		ptr_for_particles_array[i].SetY((float)(d.boundY * GenerateRandomNumber()));
	}
	return ptr_for_particles_array;
}
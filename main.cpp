#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include <random>

using namespace sf;

unsigned int getScreenWidth() //returns screen size
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	return desktop.width;
}

unsigned int getScreenHeight() //
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	return desktop.height;
}



const float PI = 3.14; //pi 
const float g = 0.10; //acceleration of free fall
const unsigned int coef = 1; //the coefficient of proportionality between the repulsive force and the masses of particles divided by the distance between them
const unsigned int boundX = getScreenWidth(); //size of the sfml window
const unsigned int boundY = getScreenHeight();
const float r = 8; //radius of a circle of a particle
const float Radius_of_Interaction = 1; //the radius of the area of interaction of this particle with the rest





long double Find_Distance(const double x1, const double y1, const double x2, const double y2) { //find the distance between the centers of two particles
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}


void sleep(unsigned int mc) {
	//delay for ms microseconds
	std::this_thread::sleep_for(std::chrono::microseconds(mc));
}


class Particle
	//the class of the gradient of the "molecule" itself and its display as a particle
	//the particle is able to attract to the earth, repel other particles and has viscosity
	//it lives in two-dimensional space
{

private:
	sf::CircleShape circle; //every particle is shown by the circle
	double vx = 0; //velocity
	double vy = 0;
	const int mass = 1; //the mass that is concentrated in the center

	//===================================================================================//
	const float a = 1; //offset 1/x^2 along the x and y axes in the Gradient_of_Interaction method
	const float alpha = 1; //the coefficient of stretching is 1 / x ^ 2 in the Gradient_of_Interaction method
	//===================================================================================//

public:
	
	Particle(float x_, float y_) { //creates a blue particle with coord x::y
		sf::Color circleColor(100, 255, 127);
		circle.setFillColor(circleColor);
		circle.setPosition(x_, y_);
		circle.setRadius(r);
	}

	Particle() : Particle(boundX / 2, boundY / 2) {} //creates a blue particle with coord boundX / 2::boundY / 2

	double GetX() const { return this->circle.getPosition().x; } //coord getters and setters
	double GetY() const { return this->circle.getPosition().y; }
	void SetX(const double x_) { double x = GetX(); double y = GetY(); this->circle.setPosition(x_, y); }
	void SetY(const double y_) { double x = GetX(); double y = GetY(); this->circle.setPosition(x, y_); }

	double GetVx() const { return this->vx; } //vertice getters and setters
	double GetVy() const { return this->vy; }
	void SetVx(const double vx_) { this->vx = vx_; }
	void SetVy(const double vy_) { this->vy = vy_; }

	void Earth_Gravity() { this->vy += g; } //its really gravity, makes the particle fall faster

	double Gradient_of_Interaction(Particle A) {
		//calculates the value of the "field" of a given particle at the point of another particle A with the coord x::y
		//returns 0 if the particle is outside the interaction radius, the value of the "gradient" otherwise
		//the magnitude of the "gradient" depends on the coordinate as 1/x^2, and the hyperbola is shifted to the right by a constant a on the x axis, down by a on the y axis and stretched alpha times
		//ro - the distance from this particle to particle A
		double ro = Find_Distance(A.GetX(), A.GetY(), this->GetX(), this->GetY());
		if (ro >= Radius_of_Interaction) return 0;
		return (alpha / ((ro + a) * (ro + a)) - a);
	}

	void move() {  //particle movenment
		double x = GetX();
		double y = GetY();
		this->circle.setPosition(x += vx, y += vy);
	}


	long double Find_speed() const { return sqrt((this->vx) * (this->vx) + (this->vy) * (this->vy)); } //calculates the total velocity of the particle


	sf::CircleShape GetCircle() const { return this->circle; } //returns the circle of the particle

	void rebound() {
		//when a particle hits a wall/floor, it bounces off, losing some of its energy
		if (this->GetY() - boundY + 15 > -5) { //the force of friction on the floor
				this->SetVx(this->GetVx() * 0.97);
			//if (abs(this->GetVx()) <= 0.1) this->SetVx(0); //if the velocity is too low, the particle stops
		}
		if (this->GetX() > boundX - 15) {
			this->SetX(boundX - 15);
			this->SetVx(-this->GetVx() * 0.7);
		}
		else if (this->GetX() < 0) {
			this->SetX(0);
			this->SetVx(-this->GetVx() * 0.7);
		}
		if (this->GetY() > boundY - 17) {
			this->SetY(boundY - 17);
			this->SetVy(-this->GetVy() * 0.7);
			if (abs(this->GetVy()) <= 0.2) this->SetVy(0); //if the velocity is too low, the particle stops
		}
	}

	void recolour() {
		//the color of the particle depends on its velocity
		int red;
		int green;
		int blue;
		if ((int)this->Find_speed() * 6 > 127) { //we won't change color if the velocity is too high
			red = 100;
			green = 255;
			blue = 127;
			return;
		}
		red = 100;
		green = 120 + (int)(this->Find_speed() * 6);
		blue = 240 - (int)(this->Find_speed() * 6);
		sf::Color circleColor(red, green, blue);
		this->circle.setFillColor(circleColor);
	}


};



void Molecular_Interaction(Particle A, Particle B) { //interaction between particles
	//THEN HERE WILL BE A GRADIENT MAP AND HASHING
	//============ FINISH WRITING ===============//
	std::cout << "The molecular interaction works!" << std::endl;
}


void add_impusle(Particle& A) { A.SetVy(-5.0); } //when we press enter, the particle gets impulse

void left_mouse_click(Particle& A, RenderWindow* window_ptr) {
	//we realize the attraction to the cursor when you click the mouse(lmb)
	//there are two options: depending on the length and on the length squared
	//we choose the second one, because we want to interact more with close particles

	if (boundY - A.GetY() <= 17) { A.SetVy(-0.5); A.SetY(boundY - 17); return; } //this fixes sticking to the floor

	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window_ptr);
	sf::Vector2f direction = sf::Vector2f(mousePosition) - A.GetCircle().getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	//direction /= (length / 0.4);
	direction = (Vector2f)(direction * 100.0f / (length * length));

	double vx = A.GetVx();
	double vy = A.GetVy();

	vx += direction.x;
	vy += direction.y;
	if (length < 40) return; //depending on the square of the length, the particles should not approach the cursor too much
	A.SetVx(vx);
	A.SetVy(vy);
}

void right_mouse_click(Particle& A, RenderWindow* window_ptr) {
	//we relize repulsion from the cursor when the mouse is clicked(rmb)
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window_ptr);
	sf::Vector2f direction = sf::Vector2f(mousePosition) - A.GetCircle().getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	direction = (Vector2f)(direction * 100.0f / (length * length));

	double vx = A.GetVx();
	double vy = A.GetVy();

	vx -= 0.3 * direction.x;
	vy -= 0.3 * direction.y;

	A.SetVx(vx);
	A.SetVy(vy);
}



double generateRandomNumber() {
	//generate random number from zero to one
	std::random_device rd;
	std::mt19937 random_number(rd());
	std::uniform_real_distribution<double> dis(0.0, 1.0);

	return dis(random_number); // Генерируем и возвращаем случайное число от 0 до 1
}

Particle** create_particle_array(const unsigned int x_number_of_particels) {
	//creates array of particles x_number_of_particels * x_number_of_particels;
	Particle** ptr_for_particles_arrays = new Particle * [x_number_of_particels];
	for (unsigned int i = 0; i < x_number_of_particels; i++) {
		ptr_for_particles_arrays[i] = new Particle[x_number_of_particels];
	}

	for (unsigned int i = 0; i < x_number_of_particels; i++) {
		for (unsigned int j = 0; j < x_number_of_particels; j++) {
			ptr_for_particles_arrays[i][j].SetX(boundX * generateRandomNumber()); //every particle from the array has random coord
			ptr_for_particles_arrays[i][j].SetY(boundY * generateRandomNumber());
		}
	}
	return ptr_for_particles_arrays;
}

void delete_particle_array(Particle** ptr_for_particles_arrays, const unsigned int x_number_of_particels) {
	//deletes array of particles x_number_of_particels * x_number_of_particels;
	for (unsigned int i = 0; i < x_number_of_particels; i++) {
		delete[] ptr_for_particles_arrays[i];
	}
	delete[] ptr_for_particles_arrays;
}



int main()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode(); //sfml window is fullscreen
	sf::RenderWindow window(desktop, "Fluid simulation", sf::Style::Fullscreen);

	unsigned int x_number_of_particels = 40; //defines the size of an array (square) filled with particles
	Particle** ptr_for_particles_arrays = create_particle_array(x_number_of_particels); //creates the array filled with particles
	

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
				window.close();
		}

		
		for (unsigned int i = 0; i < x_number_of_particels; i++) {
			for (unsigned int j = 0; j < x_number_of_particels; j++) {
				ptr_for_particles_arrays[i][j].rebound();
				ptr_for_particles_arrays[i][j].Earth_Gravity();
				ptr_for_particles_arrays[i][j].recolour();
				ptr_for_particles_arrays[i][j].move();
				ptr_for_particles_arrays[i][j].Earth_Gravity();

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) left_mouse_click(ptr_for_particles_arrays[i][j], &window); //attraction to the cursor when pressing the lmb
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) right_mouse_click(ptr_for_particles_arrays[i][j], &window); //repulsion from the cursor when pressing the rmb
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) add_impusle(ptr_for_particles_arrays[i][j]);

				
				window.draw(ptr_for_particles_arrays[i][j].GetCircle());
			}
		}
		
		window.display();
		window.clear();
		sleep(50);
		
	}
	delete_particle_array(ptr_for_particles_arrays, x_number_of_particels); //clears memory
	return 0;
}

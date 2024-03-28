#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#include <thread>
#include <iostream>
#include <random>
#include <vector>



const double PI = 3.14; //pi 
const double g = 0.050; //acceleration of free fall
const unsigned int coef = 1; //the coefficient of proportionality between the repulsive force and the masses of particles divided by the distance between them
unsigned int boundX; //size of the sfml window
unsigned int boundY;
const float r = 10; //radius of a circle of a particle
const float Radius_of_Interaction = 1; //the radius of the area of interaction of this particle with the rest
const float a = 1; //offset 1/x^2 along the x and y axes in the Gradient_of_Interaction method
const float alpha = 1; //the coefficient of stretching is 1 / x ^ 2 in the Gradient_of_Interaction method




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
public:
	
	Particle(float x_, float y_) { //creates a blue particle with coord x::y
		sf::Color circleColor(100, 255, 127);
		circle.setFillColor(circleColor);
		circle.setPosition(x_, y_);
		circle.setRadius(r);
	}

	Particle() : Particle((float)boundX / 2, (float)boundY / 2) {} //creates a blue particle with coord boundX / 2::boundY / 2

	float GetX() const { return this->circle.getPosition().x; } //coord getters and setters
	float GetY() const { return this->circle.getPosition().y; }
	void SetX(const float x_) { float x = GetX(); float y = GetY(); this->circle.setPosition(x_, y); }
	void SetY(const float y_) { float x = GetX(); float y = GetY(); this->circle.setPosition(x, y_); }

	double GetVx() const { return this->vx; } //vertice getters and setters
	double GetVy() const { return this->vy; }
	void SetVx(const double vx_) { this->vx = vx_; }
	void SetVy(const double vy_) { this->vy = vy_; }

	void Earth_Gravity() { this->vy += g; } //its really gravity, makes the particle fall faster

	void move() {  //particle movenment
		float x = GetX();
		float y = GetY();
		this->circle.setPosition(x += (float)vx, y += (float)vy);
	}


	long double Find_speed() const { return sqrt((this->vx) * (this->vx) + (this->vy) * (this->vy)); } //calculates the total velocity of the particle


	sf::CircleShape GetCircle() const { return this->circle; } //returns the circle of the particle

	void rebound() {
		//when a particle hits a wall/floor, it bounces off, losing some of its energy
		if (this->GetY() - boundY + 15 > -5) { //the force of friction on the floor
				this->SetVx(this->GetVx() * 0.97);
			//if (abs(this->GetVx()) <= 0.1) this->SetVx(0); //if the velocity is too low, the particle stops
		}
		if (this->GetX() > (float)(boundX - 15)) {
			this->SetX((float)(boundX - 15));
			this->SetVx(-this->GetVx() * 0.7);
		}
		else if (this->GetX() < 0) {
			this->SetX(0);
			this->SetVx(-this->GetVx() * 0.7);
		}
		if (this->GetY() > (float)(boundY - 17)) {
			this->SetY((float)(boundY - 17));
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

	/////////////////////////////////////////////////////////////////////////////////////////////////
	/*void repulsion(Pressure_map_cell**)
	{
		sf::Vector2<unsigned int> this_cell_number = find_cell_number(this->GetX(), this->GetY());
	}
	*/
	////////////////////////////////////////////////////////////////////////////////////////////////
};



unsigned int getScreenWidth() //returns screen size
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	return desktop.width;
}

unsigned int getScreenHeight()
{
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
	return desktop.height;
}

void sleep(int sec) { std::this_thread::sleep_for(std::chrono::seconds(sec)); }//delay for sec seconds



void left_mouse_click(Particle& A, const sf::RenderWindow* window_ptr) {
	//we realize the attraction to the cursor when you click the mouse(lmb)
	//there are two options: depending on the length and on the length squared
	//we choose the second one, because we want to interact more with close particles
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window_ptr);
	sf::Vector2f direction = sf::Vector2f(mousePosition) - A.GetCircle().getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 300) return;

	if ((float)boundY - A.GetY() <= 17) { A.SetVy(-0.5); A.SetY((float)boundY - 17); return; } //this fixes sticking to the floor

	
	//direction /= (length / 0.4);
	direction = (sf::Vector2f)(direction * 100.0f / (length * length));

	if (length < 40) return; //depending on the square of the length, the particles should not approach the cursor too much

	A.SetVx(A.GetVx() + direction.x);
	A.SetVy(A.GetVy() + direction.y);
}

void right_mouse_click(Particle& A, const sf::RenderWindow* window_ptr) {
	//we relize repulsion from the cursor when the mouse is clicked(rmb)
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window_ptr);
	sf::Vector2f direction = sf::Vector2f(mousePosition) - A.GetCircle().getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (length > 300) return;
	direction = (sf::Vector2f)(direction * 100.0f / (length * length));

	A.SetVx(A.GetVx() - 0.3 * direction.x);
	A.SetVy(A.GetVy() - 0.3 * direction.y);
}



double generateRandomNumber() {
	//generate random number from zero to one
	std::random_device rd;
	std::mt19937 random_number(rd());
	std::uniform_real_distribution<double> dis(0.0, 1.0);

	return dis(random_number); // We generate and return a random number from 0 to 1
}

Particle* create_particle_array(const unsigned int number_of_particels) {
	//creates array of particles number_of_particels
	Particle* ptr_for_particles_array = new Particle [number_of_particels];

	for (unsigned int i = 0; i < number_of_particels; i++) {
		ptr_for_particles_array[i].SetX((float)(boundX * generateRandomNumber())); //every particle from the array has random coord
		ptr_for_particles_array[i].SetY((float)(boundY * generateRandomNumber()));
	}
	return ptr_for_particles_array;
}


class Pressure_map_cell
//the pressure map consists of such cells
{
private:
	sf::Vector2f coord; // Coordinates of the cell(its upper left corner coord in pixels)
	double pressure; // Pressure value of the cell

public:
	// Constructor to initialize the cell with 0:0 coordinates and default pressure of 0
	Pressure_map_cell()
	{
		this->coord.x = 0;
		this->coord.y = 0;
		this->pressure = 0;
	}

	// Getter methods
	sf::Vector2f GetCoord() { return this->coord; }
	double GetPressure() { return this->pressure; }

	// Setter methods
	void SetCoord(float x_, float y_) { this->coord.x = x_; this->coord.y = y_; }
	void SetPressure(double pressure_) { this->pressure = pressure_; }
};

class Pressure_map
//the map is used for calculation pressure forse acting on a particle
//P.S. my dear reader, im so sorry for this shit, forgive me pls
{
private:
	sf::Vector2<unsigned int> number_of_cells; // Number of cells in the pressure map
	sf::Vector2<unsigned int> size_of_cell; // Size of each cell in pixels
	Pressure_map_cell** ptr_for_pressure_map; //pointer to the map

	unsigned int Find_first_cell_number_x(Particle A) //all the private functions below are needed to calculate the number of the cell with given coords
	{
		if (Radius_of_Interaction > A.GetX()) { return 0; }
		return (unsigned int)(A.GetX() - Radius_of_Interaction) / size_of_cell.x;
	}
	unsigned int Find_first_cell_number_y(Particle A)
	{
		if (Radius_of_Interaction > A.GetY()) { return 0; }
		return (unsigned int)(A.GetY() - Radius_of_Interaction) / size_of_cell.y;
	}
	unsigned int Find_last_cell_number_x(Particle A)
	{
		if (Radius_of_Interaction + A.GetX() > boundX) { return number_of_cells.x; }
		return (unsigned int)(A.GetX() + Radius_of_Interaction) / size_of_cell.x;
	}
	unsigned int Find_last_cell_number_y(Particle A)
	{
		if (Radius_of_Interaction + A.GetY() > boundY) { return number_of_cells.y; }
		return (unsigned int)(A.GetY() + Radius_of_Interaction) / size_of_cell.y;
	}

public:
	// Constructor to initialize the pressure map with given dimensions
	Pressure_map(unsigned int x_, unsigned int y_)
	{
		number_of_cells.x = x_;
		number_of_cells.y = y_;
		size_of_cell.x = boundX / number_of_cells.x; // Calculate size of cell in x-direction
		size_of_cell.y = boundY / number_of_cells.y; // Calculate size of cell in y-direction

		// Dynamically allocate memory for the pressure map cells
		ptr_for_pressure_map = new Pressure_map_cell * [number_of_cells.x];
		for (unsigned int i = 0; i < number_of_cells.x; i++)
		{
			ptr_for_pressure_map[i] = new Pressure_map_cell[number_of_cells.y];
			for (unsigned int j = 0; j < number_of_cells.y; j++)
			{
				// Set coordinates for each cell based on its position in the grid
				ptr_for_pressure_map[i][j].SetCoord((float)i * size_of_cell.x, (float)j * number_of_cells.y);
			}
		}
	}

	//Clears memory
	~Pressure_map()
	{
		for (unsigned i = 0; i < number_of_cells.x; i++) { delete[] ptr_for_pressure_map[i]; }
		delete[] ptr_for_pressure_map;
		ptr_for_pressure_map = nullptr;
	}

	double Find_particles_pressure(Particle A, Pressure_map_cell cell) {
		//calculates the value of the "pressure" of a given particle at the point of another particle A with the coord x::y
		//returns either the value of the "pressure" if the cell is in radius of interraction or zero if it isnt
		//the magnitude of the "gradient" depends on the coordinate as 1/x^2, and the hyperbola is shifted to the right by a constant a on the x axis, down by a on the y axis and stretched alpha times
		//ro - the distance from this particle to particle A
		double x1 = A.GetX(); //just for better readability
		double y1 = A.GetY();
		double x2 = cell.GetCoord().x;
		double y2 = cell.GetCoord().y;
		double ro = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		if (ro > Radius_of_Interaction) { return 0; }
		return (alpha / ((ro + a) * (ro + a)) - a); //this is a hyperbola shifted down and to the left
	}

	void Calculate_pressure(Particle* ptr_for_particles_array, unsigned int number_of_particels)
	//calculates pressure in every cell of the pressure map
	{
		for (unsigned i = 0; i < number_of_particels; i++) 
		{
			Particle A = ptr_for_particles_array[i]; //we precalculate it for more speed
			for (unsigned j = Find_first_cell_number_x(A); j < Find_last_cell_number_x(A); j++)
			{
				for (unsigned k = Find_first_cell_number_y(A); k < Find_last_cell_number_y(A); k++)
				{
					double delta_pressure = Find_particles_pressure(A, ptr_for_pressure_map[j][k]);
					ptr_for_pressure_map[j][k].SetPressure(ptr_for_pressure_map[j][k].GetPressure() + delta_pressure);
				}
			}
		}
	}

	Pressure_map_cell** get_pressure_map_ptr() { return ptr_for_pressure_map; } //returns pointer for pressure map

	sf::Vector2<unsigned int> find_this_cell_number(float x_, float y_) //returns a cell number situated on x_:y_ coords
	{
		sf::Vector2<unsigned int> a;
		a.x = (unsigned int)(x_ / number_of_cells.x);
		a.y = (unsigned int)(y_ / number_of_cells.y);
		return a;
	}

	sf::Vector2<unsigned int> number_of_cells() { return number_of_cells; } //returns number of cells in the pressure map
};


int main()
{
	sf::RenderWindow window;
	std::cout << "if you have linux os of windows os, enter 0, if you have mac os or smth else, enter 1:" << std::endl;
	bool os_type;
	std::cin >> os_type;
	if (os_type){ std::cout << "mac sucks" << std::endl; }
	

	std::cout << "enter number of particles (1 000-10 000 recommended):" << std::endl;
	unsigned int number_of_particels; //defines the number of particles
	std::cin >> number_of_particels;

	std::cout << "fluid simulation will start in 4 seconds. If you want to close it, press ESC or press the red cross" << std::endl;
	sleep(4);


	if (!os_type) 
	{
		boundX = getScreenWidth();
		boundY = getScreenHeight();
		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
		window.create(desktop, "Fluid simulation", sf::Style::Fullscreen); //fullscreen for windows and linux
	}
	else 
	{
		boundX = 1200;
		boundY = 800;
		window.create(sf::VideoMode(boundX, boundY), "Fluid simulation"); //bullshit for mac
	}

	
	const sf::RenderWindow* window_pointer = &window; //we precalculate it for more speed in the future

	window.setFramerateLimit(80);
	
	Particle* ptr_for_particles_array = create_particle_array(number_of_particels); //creates the array filled with particles

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed)
				window.close();
		}

		
		for (unsigned int i = 0; i < number_of_particels; i++) {
			ptr_for_particles_array[i].rebound();
			ptr_for_particles_array[i].Earth_Gravity();
			ptr_for_particles_array[i].recolour();
			ptr_for_particles_array[i].move();
			ptr_for_particles_array[i].Earth_Gravity();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) left_mouse_click(ptr_for_particles_array[i], window_pointer); //attraction to the cursor when pressing the lmb
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) right_mouse_click(ptr_for_particles_array[i], window_pointer); //repulsion from the cursor when pressing the rmb

			
			window.draw((ptr_for_particles_array[i]).GetCircle());
		}
		
		window.display();
		window.clear();
		
	}
	window.display();

	delete[] ptr_for_particles_array; //clears memory
	return 0;
}

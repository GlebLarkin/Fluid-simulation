#ifndef FLUID_PARTICLE_HPP_
#define FLUID_PARTICLE_HPP_

#include <SFML/Graphics.hpp>
#include <Class.hpp>


class Particle
	//the class of the gradient of the "molecule" itself and its display as a particle
	//the particle is able to attract to the earth, repel other particles and has viscosity
	//it lives in two-dimensional space
{
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

	void Earth_Gravity(); //its really gravity, makes the particle fall faster

	void move(); //particle movenment


	long double Find_speed() const; //calculates the total velocity of the particle


	sf::CircleShape GetCircle() const; //returns the circle of the particle

	void rebound(); //when a particle hits a wall/floor, it bounces off, losing some of its energy


	void recolour(); //the color of the particle depends on its velocity


	/*void repulsion(Pressure_map_cell** ptr)
	{
		sf::Vector2<unsigned int> cell_number = find_cell_number(this->GetX(), this->GetY());
		sf::Vector2<unsigned int> num = get_number_of_cells();
		if (cell_number.x != 0 and cell_number.y != 0 and cell_number.x != num.y - 1 and cell_number.y != num.y - 1) 
		{
			vx += (ptr[cell_number.x + 1][cell_number.y].GetPressure() - ptr[cell_number.x - 1][cell_number.y].GetPressure());
			vy += (ptr[cell_number.x][cell_number.y + 1].GetPressure() - ptr[cell_number.x][cell_number.y - 1].GetPressure());
		}
		else
		{
			if ((cell_number.y == 0 and cell_number.x != 0 and cell_number.x != num.x - 1) or (cell_number.y == num.y - 1 and cell_number.x != 0 and cell_number.x != num.x - 1))
			{
				vx += (ptr[cell_number.x + 1][cell_number.y].GetPressure() - ptr[cell_number.x - 1][cell_number.y].GetPressure());
			}
			if ((cell_number.x == 0 and cell_number.y != 0 and cell_number.y != num.y - 1) or (cell_number.x == num.x - 1 and cell_number.y != 0 and cell_number.y != num.y - 1))
			{
				vy += (ptr[cell_number.x][cell_number.y + 1].GetPressure() - ptr[cell_number.x][cell_number.y - 1].GetPressure());
			}
			}
	}*/
private:
	sf::CircleShape circle; //every particle is shown by the circle
	double vx = 0; //velocity
	double vy = 0;
	const int mass = 1; //the mass that is concentrated in the center
};

#endif  // FLUID_PARTICLE_HPP_
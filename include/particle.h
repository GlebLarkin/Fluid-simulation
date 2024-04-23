#ifndef FLUID_PARTICLE_HPP_
#define FLUID_PARTICLE_HPP_

#include <SFML/Graphics.hpp>
#include <class.h>


class Particle
	//the class of the gradient of the "molecule" itself and its display as a particle
	//the particle is able to attract to the earth, repel other particles and has viscosity
	//it lives in two-dimensional space
{
public:
	
	Particle(float x, float y, Data & d, const sf::Texture& texture) { //creates a blue particle with coord x::y
		sf::Color circleColor(100, 255, 127);
		circle_.setFillColor(circleColor);
		circle_.setPosition(x, y);
		circle_.setRadius(d.r);
		circle_.setTexture(&texture);
	}

	Particle(Data & d, const sf::Texture& texture) : Particle((float)d.boundX / 2, (float)d.boundY / 2, d, texture) {} //creates a blue particle with coord boundX / 2::boundY / 2

	Particle(const Particle &) = delete;

	float GetX() const { return this->circle_.getPosition().x; } //coord getters and setters
	float GetY() const { return this->circle_.getPosition().y; }
	void SetX(const float & x_) { float x = GetX(); float y = GetY(); this->circle_.setPosition(x_, y); }
	void SetY(const float & y_) { float x = GetX(); float y = GetY(); this->circle_.setPosition(x, y_); }

	double GetVx() const { return this->vx_; } //vertice getters and setters
	double GetVy() const { return this->vy_; }
	void SetVx(const double & vx) { this->vx_ = vx; }
	void SetVy(const double & vy) { this->vy_ = vy; }

	void EarthGravity(Data & d); //its really gravity, makes the particle fall faster

	void Move(); //particle movenment


	long double FindSpeed() const; //calculates the total velocity of the particle


	sf::CircleShape& GetCircle(); //returns the circle of the particle

	void Rebound(Data & d); //when a particle hits a wall/floor, it bounces off, losing some of its energy


	void Recolour(); //the color of the particle depends on its velocity

private:
	sf::CircleShape circle_; //every particle is shown by the circle
	double vx_ = 0; //velocity
	double vy_ = 0;
	const int mass_ = 1; //the mass that is concentrated in the center
};

#endif  // FLUID_PARTICLE_HPP_
#include "particle.h"

void Particle::EarthGravity(Data & d) { this->vy_ += d.g; } //its really gravity, makes the particle fall faster

void Particle::Move() {  //particle movenment
	vx_ *= 0.98;
	vy_ *= 0.98;
	float x = GetX();
	float y = GetY();
	this->circle_.setPosition(x += (float)vx_, y += (float)vy_);
}

long double Particle::FindSpeed() const { return sqrt((this->vx_) * (this->vx_) + (this->vy_) * (this->vy_)); } //calculates the total velocity of the particle

sf::CircleShape& Particle::GetCircle() { return this->circle_; } //returns the circle of the particle

void Particle::Rebound(Data & d) {
	//when a particle hits a wall/floor, it bounces off, losing some of its energy
	/*if (this->GetY() + 2 * d.r > d.boundY) { //the force of friction on the floor
			this->SetVx(this->GetVx() * 0.97);
		//if (abs(this->GetVx()) <= 0.1) this->SetVx(0); //if the velocity is too low, the particle stops
	}*/
	if (this->GetX() + 2 * d.r > (float)(d.boundX)) {
		this->SetX((float)(d.boundX - 2 * d.r));
		//this->SetVx(-this->GetVx() * 0.7);
		vx_ *= (-0.7);
	}
	else if (this->GetX() < 0) {
		this->SetX(0);
		//this->SetVx(-this->GetVx() * 0.7);
		vx_ *= (-0.7);
	}
	if (this->GetY() > (float)(d.boundY - 2 * d.r)) {
		this->SetY((float)(d.boundY - 2 * d.r));
		//this->SetVy(-this->GetVy() * 0.7);
		//if (abs(this->GetVy()) <= 0.2) this->SetVy(0); //if the velocity is too low, the particle stops

		vy_ *= (-0.7);
	}
	else if (this->GetY() < 0) {
		this->SetY((float)0);
		//this->SetVy(-this->GetVy() * 0.7);
		vy_ *= (-0.7);
	}
}

void Particle::Recolour() {
	//the color of the particle depends on its velocity
	int red;
	int green;
	int blue;
	if ((int)this->FindSpeed() * 12 > 127) { //we won't change color if the velocity is too high
		red = 100;
		green = 255;
		blue = 127;
		return;
	}
	red = 100;
	green = 120 + (int)(this->FindSpeed() * 12);
	blue = 240 - (int)(this->FindSpeed() * 12);
	sf::Color circleColor(red, green, blue);
	this->circle_.setFillColor(circleColor);
}
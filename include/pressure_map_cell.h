#ifndef FLUID_PRESSURE_MAP_CELL_HPP_
#define FLUID_PRESSURE_MAP_CELL_HPP_

#include <func.h>

class PressureMapCell
//the pressure map consists of such cells
{
public:
	// Constructor to initialize the cell with 0:0 coordinates and default pressure of 0
	PressureMapCell() : coord_x_(0), coord_y_(0), pressure_(0), viscosity_(0), speed_x_(0), speed_y_(0) {}

	// Getter methods
	//sf::Vector2f GetCoord();
	unsigned int GetCoordX();
	unsigned int GetCoordY();
	double GetPressure();

	double GetViscosity(){ return viscosity_; };
	double GetSpeedX(){ return speed_x_; };
	double GetSpeedY(){ return speed_y_; };

	// Setter methods
	void SetCoord(float x, float y);
	void SetPressure(double pressure);

	void SetViscosity(double param){ viscosity_ = param; };
	void SetSpeedX(double param){ speed_x_ = param; };
	void SetSpeedY(double param){ speed_y_ = param; };


private:
	unsigned int coord_x_;
	unsigned int coord_y_; // Coordinates of the cell(its upper left corner coord in pixels)

	double viscosity_;

	double speed_x_;
	double speed_y_;
	double pressure_; // Pressure value of the cell
};

#endif  // FLUID_PRESSURE_MAP_CELL_HPP_
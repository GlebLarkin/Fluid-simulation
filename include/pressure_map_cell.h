#ifndef FLUID_PRESSURE_MAP_CELL_HPP_
#define FLUID_PRESSURE_MAP_CELL_HPP_

#include <func.h>

class PressureMapCell
//the pressure map consists of such cells
{
public:
	// Constructor to initialize the cell with 0:0 coordinates and default pressure of 0
	PressureMapCell()
	{
		this->coord_x_ = 0;
		this->coord_y_ = 0;
		this->pressure_ = 0;
	}

	// Getter methods
	//sf::Vector2f GetCoord();
	unsigned int GetCoordX();
	unsigned int GetCoordY();
	double GetPressure();

	double GetViscosityX(){ return viscosity_x_; };
    double GetViscosityY(){ return viscosity_y_; };
	double GetSpeedX(){ return speed_x_; };
	double GetSpeedY(){ return speed_y_; };

	// Setter methods
	void SetCoord(float x, float y);
	void SetPressure(double pressure);

	void SetViscosityX(double param){ viscosity_x_ = param; };
    void SetViscosityY(double param){ viscosity_y_ = param; };
	void SetSpeedX(double param){ speed_x_ = param; };
	void SetSpeedY(double param){ speed_y_ = param; };


private:
	unsigned int coord_x_;
	unsigned int coord_y_; // Coordinates of the cell(its upper left corner coord in pixels)

	double viscosity_x_;
	double viscosity_y_;

	double speed_x_;
	double speed_y_;
	double pressure_; // Pressure value of the cell
};

#endif  // FLUID_PRESSURE_MAP_CELL_HPP_
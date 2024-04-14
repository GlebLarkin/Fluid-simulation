#ifndef FLUID_PRESSURE_MAP_CELL_HPP_
#define FLUID_PRESSURE_MAP_CELL_HPP_

#include <Func.hpp>

class Pressure_map_cell
//the pressure map consists of such cells
{
public:
	// Constructor to initialize the cell with 0:0 coordinates and default pressure of 0
	Pressure_map_cell()
	{
		this->coord_x = 0;
		this->coord_y = 0;
		this->pressure = 0;
	}

	// Getter methods
	//sf::Vector2f GetCoord();
	unsigned int GetCoord_x();
	unsigned int GetCoord_y();
	double GetPressure();

	double GetViscosity_x(){ return viscosity_x; };
    double GetViscosity_y(){ return viscosity_y; };
	double GetSpeed_x(){ return Speed_x; };
	double GetSpeed_y(){ return Speed_y; };

	// Setter methods
	void SetCoord(float x_, float y_);
	void SetPressure(double pressure_);

	void SetViscosity_x(double param){ viscosity_x = param; };
    void SetViscosity_y(double param){ viscosity_y = param; };
	void SetSpeed_x(double param){ Speed_x = param; };
	void SetSpeed_y(double param){ Speed_y = param; };


private:
	unsigned int coord_x;
	unsigned int coord_y; // Coordinates of the cell(its upper left corner coord in pixels)

	double viscosity_x;
	double viscosity_y;

	double Speed_x;
	double Speed_y;
	double pressure; // Pressure value of the cell
};

#endif  // FLUID_PRESSURE_MAP_CELL_HPP_
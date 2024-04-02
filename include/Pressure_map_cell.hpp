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
		this->coord.x = 0;
		this->coord.y = 0;
		this->pressure = 0;
	}

	// Getter methods
	sf::Vector2f GetCoord();
	double GetPressure();

	// Setter methods
	void SetCoord(float x_, float y_);
	void SetPressure(double pressure_);
private:
	sf::Vector2f coord; // Coordinates of the cell(its upper left corner coord in pixels)
	double pressure; // Pressure value of the cell
};

#endif  // FLUID_PRESSURE_MAP_CELL_HPP_
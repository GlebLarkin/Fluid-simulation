#ifndef FLUID_PRESSURE_MAP_HPP_
#define FLUID_PRESSURE_MAP_HPP_

#include <Pressure_map_cell.hpp>

class Pressure_map
//the map is used for calculation pressure forse acting on a particle
//P.S. my dear reader, im so sorry for this shit, forgive me pls
{
public:
	// Constructor to initialize the pressure map with given dimensions
	Pressure_map(unsigned int x_, unsigned int y_, Data d);

	//Clears memory
	~Pressure_map();

	double Find_particles_pressure(Particle A, Pressure_map_cell cell, Data d);

	void Calculate_pressure(Particle* ptr_for_particles_array, unsigned int number_of_particels, Data d);

	Pressure_map_cell** get_pressure_map_ptr(); //returns pointer for pressure map

	sf::Vector2<unsigned int> find_this_cell_number(float x_, float y_);

	sf::Vector2<unsigned int> get_number_of_cells(); //returns number of cells in the pressure map

	unsigned int find_this_cell_number_x(float x_) { return x_ / number_of_cells_x; } //returns a cell number situated on x_:y_ coords
	unsigned int find_this_cell_number_y(float y_) { return y_ / number_of_cells_y; }

	unsigned int get_number_of_cells_x() { return number_of_cells_x; }
	unsigned int get_number_of_cells_y() { return number_of_cells_y; } //returns number of cells in the pressure map

	double Find_particles_viscosity_coef_x(Particle A, Pressure_map_cell cell, Data d);

	double Find_particles_viscosity_coef_y(Particle A, Pressure_map_cell cell, Data d);

	void Calculate_pressure_map(Particle* ptr_for_particles_array, unsigned int number_of_particels, Data d);


private:
	unsigned int number_of_cells_x;
	unsigned int number_of_cells_y; // Number of cells in the pressure map

	unsigned int size_of_cell; // Size of each cell in pixels
	Pressure_map_cell** ptr_for_pressure_map; //pointer to the map

	unsigned int Find_first_cell_number_x(Particle A, unsigned int r) //all the private functions below are needed to calculate the number of the cell with given coords
	{
		if (r > A.GetX()) { return 0; }
		return (unsigned int)(A.GetX() - r) / size_of_cell;
	}
	unsigned int Find_first_cell_number_y(Particle A, unsigned int r)
	{
		if (r > A.GetY()) { return 0; }
		return (unsigned int)(A.GetY() - r) / size_of_cell;
	}
	unsigned int Find_last_cell_number_x(Particle A, Data d, unsigned int r)
	{
		if (r + A.GetX() > d.boundX) { return number_of_cells_x - 1; }
		return (unsigned int)(A.GetX() + r) / size_of_cell;
	}
	unsigned int Find_last_cell_number_y(Particle A, Data d, unsigned int r)
	{
		if (r + A.GetY() > d.boundY) { return number_of_cells_y - 1; }
		return (unsigned int)(A.GetY() + r) / size_of_cell;
	}
};

#endif  // FLUID_PRESSURE_MAP_HPP_
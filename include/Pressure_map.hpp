#ifndef FLUID_PRESSURE_MAP_HPP_
#define FLUID_PRESSURE_MAP_HPP_

#include <Pressure_map_cell.hpp>

class Pressure_map
//the map is used for calculation pressure forse acting on a particle
//P.S. my dear reader, im so sorry for this shit, forgive me pls
{
public:
	// Constructor to initialize the pressure map with given dimensions
	Pressure_map(unsigned int x_, unsigned int y_);

	//Clears memory
	~Pressure_map();

	double Find_particles_pressure(Particle A, Pressure_map_cell cell);

	void Calculate_pressure(Particle* ptr_for_particles_array, unsigned int number_of_particels);

	Pressure_map_cell** get_pressure_map_ptr(); //returns pointer for pressure map

	sf::Vector2<unsigned int> find_this_cell_number(float x_, float y_);

	sf::Vector2<unsigned int> get_number_of_cells(); //returns number of cells in the pressure map

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
};

#endif  // FLUID_PRESSURE_MAP_HPP_
#ifndef FLUID_PRESSURE_MAP_HPP_
#define FLUID_PRESSURE_MAP_HPP_

#include <pressure_map_cell.h>

class PressureMap
//the map is used for calculation pressure forse acting on a particle
//P.S. my dear reader, im so sorry for this shit, forgive me pls
{
public:
	// Constructor to initialize the pressure map with given dimensions
	PressureMap(unsigned int x, unsigned int y, Data & d);
	//Clears memory
	~PressureMap();

	double FindParticlesPressure(Particle& A, PressureMapCell& cell, Data& d);

	void CalculatePressure(Particle* ptr_for_particles_array, const unsigned int & number_of_particels, Data & d);

	PressureMapCell** GetPressureMapPtr(); //returns pointer for pressure map

	//sf::Vector2<unsigned int> find_this_cell_number(float x_, float y_);

	//sf::Vector2<unsigned int> get_number_of_cells(); //returns number of cells in the pressure map

	unsigned int FindThisCellNumberX(float x, Data & d) 
	{
		if (x + d.r <= 0) return 0;
		if (x + d.r >= d.boundX) return number_of_cells_x - 1;
		return ( x + d.r ) / size_of_cell;
	} //returns a cell number situated on x_:y_ coords
	unsigned int FindThisCellNumberY(float y, Data & d) 
	{
		if (y + d.r <= 0) return 0;
		if (y + d.r >= d.boundY) return number_of_cells_y - 1;
		return ( y + d.r ) / size_of_cell;
	}

	unsigned int GetNumberOfCellsX() { return number_of_cells_x; }
	unsigned int GetNumberOfCellsY() { return number_of_cells_y; } //returns number of cells in the pressure map

	double FindParticlesViscosityCoefX(Particle& A, PressureMapCell& cell, Data& d);

	double FindParticlesViscosityCoefY(Particle& A, PressureMapCell& cell, Data& d);

	void Calculate_pressure_map(Particle* ptr_for_particles_array, const unsigned int & number_of_particels, Data & d);


private:
	unsigned int number_of_cells_x;
	unsigned int number_of_cells_y; // Number of cells in the pressure map

	unsigned int size_of_cell; // Size of each cell in pixels
	PressureMapCell** ptr_for_pressure_map; //pointer to the map

	//all the private functions below are needed to calculate the number of the cell with given coords
	unsigned int FindFirstCellNumberX(Particle& A, const unsigned int & r, Data& d) 
	{
		if (r > A.GetX() + d.r) { return 0; }
		return (A.GetX() + d.r - r) / size_of_cell;
	}
	unsigned int FindFirstCellNumberY(Particle& A, const unsigned int & r, Data& d)
	{
		if (r > A.GetY() + d.r) { return 0; }
		return (A.GetY() + d.r - r) / size_of_cell;
	}
	unsigned int FindLastCellNumberX(Particle& A, Data& d, const unsigned int & r)
	{
		if (r + A.GetX() + d.r > d.boundX) { return number_of_cells_x - 1; }
		return (A.GetX() + d.r + r) / size_of_cell;
	}
	unsigned int FindLastCellNumberY(Particle& A, Data& d, const unsigned int & r)
	{
		if (r + A.GetY() + d.r > d.boundY) { return number_of_cells_y - 1; }
		return (A.GetY() + d.r + r) / size_of_cell;
	}
};

void Repulsion(Particle& particle, PressureMap& pressure_map, Data& d);



#endif  // FLUID_PRESSURE_MAP_HPP_
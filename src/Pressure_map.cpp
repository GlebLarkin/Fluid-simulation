#include <Pressure_map.hpp>



	// Constructor to initialize the pressure map with given dimensions
	Pressure_map::Pressure_map(unsigned int x_, unsigned int y_)
	{
		number_of_cells.x = x_;
		number_of_cells.y = y_;
		size_of_cell.x = boundX / number_of_cells.x; // Calculate size of cell in x-direction
		size_of_cell.y = boundY / number_of_cells.y; // Calculate size of cell in y-direction

		// Dynamically allocate memory for the pressure map cells
		ptr_for_pressure_map = new Pressure_map_cell * [number_of_cells.x];
		for (unsigned int i = 0; i < number_of_cells.x; i++)
		{
			ptr_for_pressure_map[i] = new Pressure_map_cell[number_of_cells.y];
			for (unsigned int j = 0; j < number_of_cells.y; j++)
			{
				// Set coordinates for each cell based on its position in the grid
				ptr_for_pressure_map[i][j].SetCoord((float)i * size_of_cell.x, (float)j * number_of_cells.y);
			}
		}
	}

	//Clears memory
	Pressure_map::~Pressure_map()
	{
		for (unsigned i = 0; i < number_of_cells.x; i++) { delete[] ptr_for_pressure_map[i]; }
		delete[] ptr_for_pressure_map;
		ptr_for_pressure_map = nullptr;
	}

	double Pressure_map::Find_particles_pressure(Particle A, Pressure_map_cell cell) {
		//calculates the value of the "pressure" of a given particle at the point of another particle A with the coord x::y
		//returns either the value of the "pressure" if the cell is in radius of interraction or zero if it isnt
		//the magnitude of the "gradient" depends on the coordinate as 1/x^2, and the hyperbola is shifted to the right by a constant a on the x axis, down by a on the y axis and stretched alpha times
		//ro - the distance from this particle to particle A
		double x1 = A.GetX(); //just for better readability
		double y1 = A.GetY();
		double x2 = cell.GetCoord().x;
		double y2 = cell.GetCoord().y;
		double ro = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		if (ro > Radius_of_Interaction) { return 0; }
		return (alpha / ((ro + a) * (ro + a)) - a); //this is a hyperbola shifted down and to the left
	}

	void Pressure_map::Calculate_pressure(Particle* ptr_for_particles_array, unsigned int number_of_particels)
	//calculates pressure in every cell of the pressure map
	{
		for (unsigned i = 0; i < number_of_particels; i++) 
		{
			Particle A = ptr_for_particles_array[i]; //we precalculate it for more speed
			for (unsigned j = Find_first_cell_number_x(A); j < Find_last_cell_number_x(A); j++)
			{
				for (unsigned k = Find_first_cell_number_y(A); k < Find_last_cell_number_y(A); k++)
				{
					double delta_pressure = Find_particles_pressure(A, ptr_for_pressure_map[j][k]);
					ptr_for_pressure_map[j][k].SetPressure(ptr_for_pressure_map[j][k].GetPressure() + delta_pressure);
				}
			}
		}
	}

	Pressure_map_cell** Pressure_map::get_pressure_map_ptr() { return ptr_for_pressure_map; } //returns pointer for pressure map

	sf::Vector2<unsigned int> Pressure_map::find_this_cell_number(float x_, float y_) //returns a cell number situated on x_:y_ coords
	{
		sf::Vector2<unsigned int> a;
		a.x = (unsigned int)(x_ / number_of_cells.x);
		a.y = (unsigned int)(y_ / number_of_cells.y);
		return a;
	}

	sf::Vector2<unsigned int> Pressure_map::get_number_of_cells() { return number_of_cells; } //returns number of cells in the pressure map

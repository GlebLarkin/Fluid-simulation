#include <Pressure_map.hpp>



	// Constructor to initialize the pressure map with given dimensions
	Pressure_map::Pressure_map(unsigned int x_, unsigned int y_, Data d): size_of_cell(10), number_of_cells_x(x_), number_of_cells_y(y_)
	{
		/*
		number_of_cells_x = x_;
		number_of_cells_y = y_;
		size_of_cell.x = d.boundX / number_of_cells.x; // Calculate size of cell in x-direction
		size_of_cell.y = d.boundY / number_of_cells.y; // Calculate size of cell in y-direction
		*/
		// Dynamically allocate memory for the pressure map cells
		ptr_for_pressure_map = new Pressure_map_cell * [number_of_cells_x];
		for (unsigned int i = 0; i < number_of_cells_x; i++)
		{
			ptr_for_pressure_map[i] = new Pressure_map_cell[number_of_cells_y];
			for (unsigned int j = 0; j < number_of_cells_y; j++)
			{
				// Set coordinates for each cell based on its position in the grid
				ptr_for_pressure_map[i][j].SetCoord((float)i * size_of_cell, (float)j * number_of_cells_y);
			}
		}
	}

	//Clears memory
	Pressure_map::~Pressure_map()
	{
		for (unsigned int i = 0; i < number_of_cells_x; i++) { delete[] ptr_for_pressure_map[i]; }
		delete[] ptr_for_pressure_map;
		//ptr_for_pressure_map = nullptr;
	}

	double Pressure_map::Find_particles_pressure(Particle A, Pressure_map_cell cell, Data d) { //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! fix pressure function
		//calculates the value of the "pressure" of a given particle at the point of another particle A with the coord x::y
		//returns either the value of the "pressure" if the cell is in radius of interraction or zero if it isnt
		//the magnitude of the "gradient" depends on the coordinate as 1/x^2, and the hyperbola is shifted to the right by a constant a on the x axis, down by a on the y axis and stretched alpha times
		//ro - the distance from this particle to particle A
		double x1 = A.GetX(); //just for better readability
		double y1 = A.GetY();
		double x2 = cell.GetCoord_x();
		double y2 = cell.GetCoord_y();
		double ro = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		if (ro > d.Radius_of_Interaction) { return 0; }
		return (d.alpha / ((ro + d.a) * (ro + d.a)) - d.a); //this is a hyperbola shifted down and to the left
	}
/*
	void Pressure_map::Calculate_pressure(Particle* ptr_for_particles_array, unsigned int number_of_particels, Data d)
	//calculates pressure in every cell of the pressure map
	{
		//we always set pressure icols zero before calculations
		for (unsigned int i = 0; i < number_of_cells_x; i++)
		{
			for (unsigned int j = 0; j < number_of_cells_y; j++)
			{
				ptr_for_pressure_map[i][j].SetPressure(0);
			}
		}
		//now we are ready to calculate pressure
		for (unsigned int i = 0; i < number_of_particels; i++) 
		{
			//Particle A = ptr_for_particles_array[i]; //we precalculate it for more speed
			for (unsigned int j = Find_first_cell_number_x(ptr_for_particles_array[i], d); j < Find_last_cell_number_x(ptr_for_particles_array[i], d); j++)
			{
				for (unsigned int k = Find_first_cell_number_y(ptr_for_particles_array[i], d); k < Find_last_cell_number_y(ptr_for_particles_array[i], d); k++)
				{
					double delta_pressure = Find_particles_pressure(ptr_for_particles_array[i], ptr_for_pressure_map[j][k], d);
					ptr_for_pressure_map[j][k].SetPressure(ptr_for_pressure_map[j][k].GetPressure() + delta_pressure);
				}
			}
		}
	}
 */

	Pressure_map_cell** Pressure_map::get_pressure_map_ptr() { return ptr_for_pressure_map; } //returns pointer for pressure map

/*
	sf::Vector2<unsigned int> Pressure_map::find_this_cell_number(float x_, float y_) //returns a cell number situated on x_:y_ coords
	{
		sf::Vector2<unsigned int> a;
		a.x = (unsigned int)(x_ / number_of_cells_x);
		a.y = (unsigned int)(y_ / number_of_cells_y);
		return a;
	}
*/



	double Pressure_map::Find_particles_viscosity_coef_x(Particle A, Pressure_map_cell cell, Data d) {
		//calculates the value of the "viscosity" of a given particle at the point of another particle A with the coord x::y
		//returns either the value of the "viscosity" if the cell is in radius of viscosity or zero if it isnt
		//the magnitude of the "gradient" depends on the coordinate as 1/x^2, and the hyperbola is shifted to the right by a constant a on the x axis, down by a on the y axis and stretched alpha times
		//ro - the distance from this particle to particle A
		cell.SetSpeed_x(cell.GetSpeed_x() + A.GetVx());
		double x1 = A.GetX();
		double y1 = A.GetY();
		double x2 = cell.GetCoord_x();
		double y2 = cell.GetCoord_y();
		double ro = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		if (ro > d.Radius_of_Viscosity) { return 0; }
		return (1 * abs(x1 - x2)/ ((ro + 1) * (ro + 1) * (ro + 1)) - 1); //this is a hyperbola shifted down and to the left
	}




	double Pressure_map::Find_particles_viscosity_coef_y(Particle A, Pressure_map_cell cell, Data d) {
		//calculates the value of the "viscosity" of a given particle at the point of another particle A with the coord x::y
		//returns either the value of the "viscosity" if the cell is in radius of viscosity or zero if it isnt
		//the magnitude of the "gradient" depends on the coordinate as 1/x^2, and the hyperbola is shifted to the right by a constant a on the x axis, down by a on the y axis and stretched alpha times
		//ro - the distance from this particle to particle A
		cell.SetSpeed_y(cell.GetSpeed_y() + A.GetVy());
		double x1 = A.GetX();
		double y1 = A.GetY();
		double x2 = cell.GetCoord_x();
		double y2 = cell.GetCoord_y();
		double ro = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		if (ro > d.Radius_of_Viscosity || ro < size_of_cell / 2) { return 0; } //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		return (1 * abs(y1 - y2)/ ((ro + 1) * (ro + 1) * (ro + 1)) - 1); //this is a hyperbola shifted down and to the left
	}




void Pressure_map::Calculate_pressure_map(Particle* ptr_for_particles_array, unsigned int number_of_particels, Data d) //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	//calculates pressure and viscosity in every cell of the pressure map
	{
		unsigned int Radius_of_Interaction = d.Radius_of_Interaction; //we precalculate them for more speed
		unsigned int Radius_of_Viscosity = d.Radius_of_Viscosity;

		//we always set pressure, speed vector and viscosity vector icol zero before calculations
		for (unsigned int i = 0; i < number_of_cells_x; i++)
		{
			for (unsigned int j = 0; j < number_of_cells_y; j++)
			{
				ptr_for_pressure_map[i][j].SetPressure(0);
				ptr_for_pressure_map[i][j].SetViscosity_x(0);
				ptr_for_pressure_map[i][j].SetViscosity_y(0);
				ptr_for_pressure_map[i][j].SetSpeed_x(0);
				ptr_for_pressure_map[i][j].SetSpeed_y(0);
			}
		}

		//now we are ready to calculate pressure
		for (unsigned int i = 0; i < number_of_particels; i++) 
		{
			for (unsigned int j = Find_first_cell_number_x(ptr_for_particles_array[i], Radius_of_Interaction); j < Find_last_cell_number_x(ptr_for_particles_array[i], d, Radius_of_Interaction); j++)
			{
				for (unsigned int k = Find_first_cell_number_y(ptr_for_particles_array[i], Radius_of_Interaction); k < Find_last_cell_number_y(ptr_for_particles_array[i], d, Radius_of_Interaction); k++)
				{
					double delta_pressure = Find_particles_pressure(ptr_for_particles_array[i], ptr_for_pressure_map[j][k], d);
					ptr_for_pressure_map[j][k].SetPressure(ptr_for_pressure_map[j][k].GetPressure() + delta_pressure);


					double delta_speed_x = ptr_for_particles_array[i].GetVx() - ptr_for_pressure_map[j][k].GetSpeed_x();
					double delta_speed_y = ptr_for_particles_array[i].GetVy() - ptr_for_pressure_map[j][k].GetSpeed_y();

					double delta_viscosity_x = delta_speed_x * Find_particles_viscosity_coef_x(ptr_for_particles_array[i], ptr_for_pressure_map[j][k], d);
					double delta_viscosity_y = delta_speed_y * Find_particles_viscosity_coef_y(ptr_for_particles_array[i], ptr_for_pressure_map[j][k], d);

					ptr_for_pressure_map[j][k].SetViscosity_x(ptr_for_pressure_map[j][k].GetViscosity_x() + delta_viscosity_x);
					ptr_for_pressure_map[j][k].SetViscosity_y(ptr_for_pressure_map[j][k].GetViscosity_y() + delta_viscosity_y);
				}
			}
		}
	}


/*
void Particle::repulsion(Pressure_map& pressure_map, Data d) //now all the particles know about each other!
{
    Pressure_map_cell** ptr = pressure_map.get_pressure_map_ptr();

    unsigned int number_of_cells_x = pressure_map.get_number_of_cells_x();
    unsigned int number_of_cells_y = pressure_map.get_number_of_cells_y();

    unsigned int cell_number_x = pressure_map.find_this_cell_number_x(this->GetX() + d.r); //!!!!!!!! r is radius of a particle
    unsigned int cell_number_y = pressure_map.find_this_cell_number_y(this->GetY());

    if (cell_number_x != 0 and cell_number_y != 0 and cell_number_x != number_of_cells_x - 1 and cell_number_y != number_of_cells_y - 1)
    {
        vx += (ptr[cell_number_x + 1][cell_number_y].GetPressure() - ptr[cell_number_x - 1][cell_number_y].GetPressure());
        vy += (ptr[cell_number_x][cell_number_y + 1].GetPressure() - ptr[cell_number_x][cell_number_y - 1].GetPressure());
    }
    else
    {
        if ((cell_number_y == 0 and cell_number_x != 0 and cell_number_x != number_of_cells_x - 1) or (cell_number_y == number_of_cells_y - 1 and cell_number_x != 0 and cell_number_x != number_of_cells_x - 1))
        {
            vx += (ptr[cell_number_x + 1][cell_number_y].GetPressure() - ptr[cell_number_x - 1][cell_number_y].GetPressure());
        }
        if ((cell_number_x == 0 and cell_number_y != 0 and cell_number_y != number_of_cells_y - 1) or (cell_number_x == number_of_cells_x - 1 and cell_number_y != 0 and cell_number_y != number_of_cells_y - 1))
        {
            vy += (ptr[cell_number_x][cell_number_y + 1].GetPressure() - ptr[cell_number_x][cell_number_y - 1].GetPressure());
        }
    }
	//viscosity forse
    vx += ptr[cell_number_x][cell_number_y].GetViscosity_x();
    vy += ptr[cell_number_x][cell_number_y].GetViscosity_y();
}
*/
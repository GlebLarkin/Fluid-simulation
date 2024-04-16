#include <iostream>
#include <fstream>
#include <chrono>
#include <Class.hpp>
#include <Func.hpp>
#include <Particle.hpp>
#include <Pressure_map_cell.hpp>
#include <Pressure_map.hpp>
#include "nlohmann_json/json.hpp"


int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Usage: ../examples//config.json\n";
        std::exit(1);
    }

    std::ifstream cfg(argv[1]);
    nlohmann::json config = nlohmann::json::parse(cfg);
    Data d{config};

	sf::RenderWindow window;
	std::cout << "if you have linux os of windows os, enter 0, if you have mac os or smth else, enter 1:" << std::endl;
	bool os_type = 1;
	//std::cin >> os_type;
	//if (os_type){ std::cout << "mac sucks" << std::endl; }
	

	std::cout << "enter number of particles (1 000-10 000 recommended):" << std::endl;
	unsigned int number_of_particels = 10; //defines the number of particles
	//std::cin >> number_of_particels;

	//std::cout << "fluid simulation will start in 4 seconds. If you want to close it, press ESC or press the red cross" << std::endl;
	//sleep(4);


	if (!os_type) 
	{
		d.boundX = getScreenWidth();
		d.boundY = getScreenHeight();
		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
		window.create(desktop, "Fluid simulation", sf::Style::Fullscreen); //fullscreen for windows and linux
	}
	else 
	{
		d.boundX = 1200;
		d.boundY = 800;
		window.create(sf::VideoMode(d.boundX, d.boundY), "Fluid simulation"); //bullshit for mac
	}

	
	
	const sf::RenderWindow* window_pointer = &window; //we precalculate it for more speed in the future

	window.setFramerateLimit(80);
	
	Particle* ptr_for_particles_array = create_particle_array(number_of_particels, d); //creates the array filled with particles

	Pressure_map map(120, 80, d);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed)
				window.close();
		}

		//map.Calculate_pressure_map(ptr_for_particles_array, number_of_particels, d);
		for (unsigned int i = 0; i < number_of_particels; i++) {
			map.Calculate_pressure_map(ptr_for_particles_array, number_of_particels, d); // Fucking shit doesnt work :/
			ptr_for_particles_array[i].rebound(d);
			repulsion(ptr_for_particles_array[i], map, d);
			ptr_for_particles_array[i].move();
			//ptr_for_particles_array[i].Earth_Gravity(d);
			ptr_for_particles_array[i].recolour();
			ptr_for_particles_array[i].move();
			//ptr_for_particles_array[i].Earth_Gravity(d);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) left_mouse_click(ptr_for_particles_array[i], window_pointer, d); //attraction to the cursor when pressing the lmb
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) right_mouse_click(ptr_for_particles_array[i], window_pointer); //repulsion from the cursor when pressing the rmb

			window.draw((ptr_for_particles_array[i]).GetCircle());
		}
		/*
		std::cout << "---------------------------------------------------------------------------------------------------------------------------------------\n";

		for (unsigned int i = 0; i < 120; ++i){
			for (unsigned int j = 0; j < 80; ++j){
				std::cout << map.get_pressure_map_ptr()[i][j].GetPressure() << " ";
			}
			std::cout << "\n";
		}
		std::cout << "---------------------------------------------------------------------------------------------------------------------------------------\n";
		
		for (unsigned int i = 0; i < 120; ++i){
			for (unsigned int j = 0; j < 80; ++j){
				std::cout << map.get_pressure_map_ptr()[i][j].GetViscosity_x() << " ";
			}
			std::cout << "\n";
		}
		std::cout << "---------------------------------------------------------------------------------------------------------------------------------------\n";
		*/
		window.display();
		window.clear();

		//sleep(10);
	}
	window.display();
	
	std::free(ptr_for_particles_array);

	delete[] ptr_for_particles_array; //clears memory
	return 0;
}

#include <iostream>
#include <fstream>
#include <chrono>
#include <class.h>
#include <func.h>
#include <particle.h>
#include <pressure_map_cell.h>
#include <pressure_map.h>
#include <barrier.h>
#include "nlohmann_json/json.hpp"


void Function(unsigned int number_of_particles, unsigned int& start_mas, unsigned int& end_mas, Data& d, PressureMap& map,
			  Particle* ptr_for_particles_array, const sf::RenderWindow* window_pointer,
			  sf::RenderWindow& window, std::mutex& mut, Barrier<NullCallback>& bar){
	while(true)
    {
		bar.ArriveAndWait();
		if(Exit) break;
		for (unsigned int i = start_mas; i < end_mas; i++) {
			Repulsion(ptr_for_particles_array[i], map, d);
			ptr_for_particles_array[i].Rebound(d);
			ptr_for_particles_array[i].Move();
			ptr_for_particles_array[i].EarthGravity(d);

			ptr_for_particles_array[i].Recolour();
			ptr_for_particles_array[i].Move();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) LeftMouseClick(ptr_for_particles_array[i], window_pointer, d); //attraction to the cursor when pressing the lmb
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) RightMouseClick(ptr_for_particles_array[i], window_pointer); //repulsion from the cursor when pressing the rmb

			do{
				std::scoped_lock sl(mut);
				window.draw((ptr_for_particles_array[i]).GetCircle());
			} while(false);
		}
		bar.ArriveAndWait();
	}
};



int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Usage: ../examples//config.json\n";
        std::exit(1);
    }

    std::ifstream cfg(argv[1]);
    nlohmann::json config = nlohmann::json::parse(cfg);
    Data d{config};

	sf::RenderWindow window;
	//std::cout << "if you have linux os of windows os, enter 0, if you have mac os or smth else, enter 1:" << std::endl;
	bool os_type = 1;
	//std::cin >> os_type;
	//if (os_type){ std::cout << "mac sucks" << std::endl; }
	

	//std::cout << "enter number of particles (1 000-10 000 recommended):" << std::endl;
	unsigned int number_of_particles = 2000; //defines the number of particles
	//std::cin >> number_of_particels;

	//std::cout << "fluid simulation will start in 4 seconds. If you want to close it, press ESC or press the red cross" << std::endl;
	//sleep(4);
	

	if (!os_type)
	{
		d.boundX = GetScreenWidth();
		d.boundY = GetScreenHeight();
		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
		window.create(desktop, "Fluid simulation", sf::Style::Fullscreen); //fullscreen for windows and linux
	}
	else 
	{
		d.boundX = 1200;
		d.boundY = 800;
		window.create(sf::VideoMode(d.boundX, d.boundY), "Fluid simulation"); //bullshit for mac
	}


	const int textureSize = 2 * d.r;
    sf::RenderTexture renderTexture;
    renderTexture.create(textureSize, textureSize);

    sf::Image image;
    image.create(textureSize, textureSize);

    for (int y = 0; y < textureSize; ++y) {
        for (int x = 0; x < textureSize; ++x) {
            float dx = x - d.r;
            float dy = y - d.r;
            float distance = std::sqrt(dx * dx + dy * dy);
            float alpha = 255 * (1 - distance / d.r);
            alpha = std::max(0.0f, std::min(255.0f, alpha));
            image.setPixel(x, y, sf::Color(255, 255, 255, static_cast<sf::Uint8>(alpha)));
        }
    }

    sf::Texture texture;
    texture.loadFromImage(image);

	
	
	const sf::RenderWindow* window_pointer = &window; //we precalculate it for more speed in the future

	window.setFramerateLimit(80);
	
	Particle* ptr_for_particles_array = CreateParticleArray(number_of_particles, d, texture); //creates the array filled with particles

	PressureMap map(120, 80, d);

	//--------------------------------- THREADS -------------------------------------

	unsigned int NUM_THREADS = 10;
	std::mutex mut;

    Barrier bar(NUM_THREADS + 1, NullCallback{});

	std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);

	unsigned int *start_mas {new unsigned int[10]{ 0, 200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800}};
	unsigned int *end_mas {new unsigned int[10]{200, 400, 600, 800, 1000, 1200, 1400, 1600, 1800, 2000}};

	for (size_t i = 0; i < NUM_THREADS; ++i) {
        threads.push_back(std::thread(Function, std::ref(number_of_particles), std::ref(start_mas[i]), std::ref(end_mas[i]), std::ref(d), std::ref(map), std::ref(ptr_for_particles_array), std::ref(window_pointer), std::ref(window), std::ref(mut), std::ref(bar)));
    }

    	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed)
				window.close();
		}

		map.Calculate_pressure_map(ptr_for_particles_array, number_of_particles, d); // Fucking shit doesnt work :/

		bar.ArriveAndWait();

		//function(number_of_particels, d, map, ptr_for_particles_array, window_pointer, window);

		//map.Calculate_pressure_map(ptr_for_particles_array, number_of_particels, d);
		/*
		for (unsigned int i = 0; i < number_of_particels; i++) {
			repulsion(ptr_for_particles_array[i], map, d);
			ptr_for_particles_array[i].rebound(d);
			ptr_for_particles_array[i].move();
			ptr_for_particles_array[i].Earth_Gravity(d);
			//ptr_for_particles_array[i].recolour();
			ptr_for_particles_array[i].move();			
			

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) left_mouse_click(ptr_for_particles_array[i], window_pointer, d); //attraction to the cursor when pressing the lmb
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) right_mouse_click(ptr_for_particles_array[i], window_pointer); //repulsion from the cursor when pressing the rmb

			window.draw((ptr_for_particles_array[i]).GetCircle());
		}
		*/

		bar.ArriveAndWait();
		
		std::cout << "---------------------------------------------------------------------------------------------------------------------------------------\n";

		for (unsigned int i = 0; i < map.GetNumberOfCellsX(); ++i){
			for (unsigned int j = 0; j < map.GetNumberOfCellsY(); ++j){
				if (map.GetPressureMapPtr()[i][j].GetPressure() != 0) {std:: cout << "o" << " "; }
				else { std::cout << "." << " "; }
				//std::cout << map.get_pressure_map_ptr()[i][j].GetPressure() << " ";
			}
			std::cout << "\n";
		}
		std::cout << "---------------------------------------------------------------------------------------------------------------------------------------\n";
		/*for (unsigned int i = 0; i < 120; ++i){
			for (unsigned int j = 0; j < 80; ++j){
				std::cout << map.get_pressure_map_ptr()[i][j].GetViscosity_x() << " ";
			}
			std::cout << "\n";
		}
		std::cout << "---------------------------------------------------------------------------------------------------------------------------------------\n";
		*/
		
		window.display();
		window.clear();

	}
	window.display();
	Exit = true;

	bar.ArriveAndWait();

	for (size_t i = 0; i < NUM_THREADS; ++i) {
        threads[i].join();
    }
	
	std::free(ptr_for_particles_array);

	delete[] ptr_for_particles_array; //clears memory
	return 0;
}
#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
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
	bool os_type = 1;
	
	unsigned int number_of_particles = 1500; //defines the number of particles
	
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

	PressureMap map(1200/4, 800/4, d);

	//--------------------------------- THREADS -------------------------------------

	unsigned int MAXIMUM_THREADS = std::thread::hardware_concurrency();
	unsigned int NUM_THREADS;
	unsigned int ONE_THREAD_CAPACITY = 150;

	unsigned int *start_mas = new unsigned int[10];
	unsigned int *end_mas = new unsigned int[10];

	if (MAXIMUM_THREADS >= 10){
		NUM_THREADS = 10;
		for (unsigned int i = 0; i < NUM_THREADS; i++){
			start_mas[i] = i * ONE_THREAD_CAPACITY;
			end_mas[i] = ONE_THREAD_CAPACITY + i * ONE_THREAD_CAPACITY;
		}
	}
	else{
		NUM_THREADS = MAXIMUM_THREADS;
		for (unsigned int i = 0; i < NUM_THREADS; i++){
			start_mas[i] = i * ONE_THREAD_CAPACITY;
			end_mas[i] = ONE_THREAD_CAPACITY + i * ONE_THREAD_CAPACITY;
		}
	}

	std::mutex mut;

    Barrier bar(NUM_THREADS + 1, NullCallback{});

	std::vector<std::thread> threads;
    threads.reserve(NUM_THREADS);

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

		bar.ArriveAndWait();

		//Map checkout, very important instrument to chech, whether pressure map works the way it shold
		//To turn on map in terminal uncomment code below
		/* 
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
		for (unsigned int i = 0; i < 120; ++i){
			for (unsigned int j = 0; j < 80; ++j){
				std::cout << map.GetPressureMapPtr()[i][j].GetViscosity() << " ";
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

	delete [] start_mas;
	delete [] end_mas;

	std::free(ptr_for_particles_array);

	delete[] ptr_for_particles_array; //clears memory for particle array
	return 0;
}
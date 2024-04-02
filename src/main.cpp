#include <iostream>

#include <Class.hpp>
#include <Particle.hpp>
#include <Func.hpp>
#include <Pressure_map_cell.hpp>


int main()
{
	sf::RenderWindow window;
	std::cout << "if you have linux os of windows os, enter 0, if you have mac os or smth else, enter 1:" << std::endl;
	bool os_type;
	std::cin >> os_type;
	if (os_type){ std::cout << "mac sucks" << std::endl; }
	

	std::cout << "enter number of particles (1 000-10 000 recommended):" << std::endl;
	unsigned int number_of_particels; //defines the number of particles
	std::cin >> number_of_particels;

	std::cout << "fluid simulation will start in 4 seconds. If you want to close it, press ESC or press the red cross" << std::endl;
	sleep(4);


	if (!os_type) 
	{
		boundX = getScreenWidth();
		boundY = getScreenHeight();
		sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
		window.create(desktop, "Fluid simulation", sf::Style::Fullscreen); //fullscreen for windows and linux
	}
	else 
	{
		boundX = 1200;
		boundY = 800;
		window.create(sf::VideoMode(boundX, boundY), "Fluid simulation"); //bullshit for mac
	}

	
	const sf::RenderWindow* window_pointer = &window; //we precalculate it for more speed in the future

	window.setFramerateLimit(80);
	
	Particle* ptr_for_particles_array = create_particle_array(number_of_particels); //creates the array filled with particles

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape || event.type == sf::Event::Closed)
				window.close();
		}

		
		for (unsigned int i = 0; i < number_of_particels; i++) {
			ptr_for_particles_array[i].rebound();
			ptr_for_particles_array[i].Earth_Gravity();
			ptr_for_particles_array[i].recolour();
			ptr_for_particles_array[i].move();
			ptr_for_particles_array[i].Earth_Gravity();

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) left_mouse_click(ptr_for_particles_array[i], window_pointer); //attraction to the cursor when pressing the lmb
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) right_mouse_click(ptr_for_particles_array[i], window_pointer); //repulsion from the cursor when pressing the rmb

			
			window.draw((ptr_for_particles_array[i]).GetCircle());
		}
		
		window.display();
		window.clear();
		
	}
	window.display();

	delete[] ptr_for_particles_array; //clears memory
	return 0;
}

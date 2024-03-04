#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>
#include <set>
#include <iostream>

using namespace sf;

//================================================//
//  !!!ПОСЛЕ ТЕСТОВ ЗАКОММЕНТИРОВАТЬ ВСЕ СOUT!!!
//================================================//



//возможно надо заменить double на float, чтобы было быстрее
const double PI = 3.14; //число пи 
const double g = 0.10; //ускорение свободного падения
const int coef = 1; //коэф пропорциональности между силой отталкивания и массами частиц деленными на расстояние между ними
const int boundX = 1200; //размеры открываемого окна в sfml
const int boundY = 800; //размеры открываемого окна в sfml
const float Radius_of_Interaction = 1; //радиус области взаимодействия данной частицы с остальными



long double Find_Distance(double x1, double y1, double x2, double y2) { //ищет расстояние между центрами двух частиц
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void sleep(int ms) {
	//задержка на ms миллисекунд
	std::this_thread::sleep_for(std::chrono::microseconds(ms));
}


class Particle
	//класс самого градиента "молекулы" и ее отображения в виде частицы
	//умеет притягиваться к земле, отталкиваться от других частиц и обладает вязкостью
	//обитает в двумерном простратстве (пока что)
{

private:
	sf::CircleShape circle; //каждая частица отображается кругом 
	const double r = 10; //радиус отображаемой частицы
	double vx = 0; //скорости
	double vy = 0;
	const int mass = 1; //масса, сосредоточенная в центре

	//===================================================================================//
	const float a = 1; //смещение 1/x^2 по осям x и у в методе Gradient_of_Interaction
	const float alpha = 1; //коэф растяжения 1/x^2 в методе Gradient_of_Interaction
	//===================================================================================//

public:
	Particle() { //создает голубую частицу с коорд 0::0
		sf::Color circleColor(100, 255, 127);
		circle.setFillColor(circleColor);
		circle.setPosition(0, 0);
		circle.setRadius(r);
		std::cout << "голубая частица с координатами" << 0 << "::" << 0 << "создалась" << std::endl;
	}
	Particle(float x_, float y_) { //создает частицу с коорд х::у
		sf::Color circleColor(100, 255, 127);
		circle.setFillColor(circleColor);
		circle.setPosition(x_, y_);
		circle.setRadius(r);
		std::cout << "голубая частица с координатами" << x_ << "::" << y_ << "создалась" << std::endl;
	}

	double GetX() { return this->circle.getPosition().x; } //геттеры и сеттеры для коорд
	double GetY() { return this->circle.getPosition().y; } //по сути они уже встроены в sfml, но так код получается слишком длинный(
	void SetX(double x_) { double x = GetX(); double y = GetY(); this->circle.setPosition(x_, y); }
	void SetY(double y_) { double x = GetX(); double y = GetY(); this->circle.setPosition(x, y_); }

	double GetVx() { return this->vx; } //геттеры и сеттеры для скоростей
	double GetVy() { return this->vy; }
	void SetVx(double vx_) { this->vx = vx_; }
	void SetVy(double vy_) { this->vy = vy_; }

	void Earth_Gravity() { this->vy += g; } //гравитация, притягивает частицу вниз

	double Gradient_of_Interaction(Particle A) {
		//рассчитывает величину "поля" данной частицы в точке другой частицы A с коорд x::y
		//возвращает 0, если частица вне радиуса взаимодействия, величину "градиента" в противном случае
		//величина "градиента" зависит от координаты как 1/x^2, причем гипербола смещена вправо на константу a по оси х, вниз на а по оси у и растянута в альфа раз
		//!АЛЬФУ В ФОРМУЛЕ НУЖНО ПОМЕНЯТЬ НА НЕОБХОДИМОЕ ЗНАЧЕНИЕ(СЕЙЧАС ОНА ЕДИНИЦА), ОТ НЕЕ ЗАВИСЯТ СВОЙСТВА ЖИДКОСТИ
		//ro - расстояние от данной частицы до частицы А
		double ro = Find_Distance(A.GetX(), A.GetY(), this->GetX(), this->GetY());
		if (ro >= Radius_of_Interaction) return 0;
		return (alpha / ((ro + a) * (ro + a)) - a);
	}

	void move() {  //движение частицы
		double x = GetX();
		double y = GetY();
		this->circle.setPosition(x += vx, y += vy); 
	}


	long double Find_speed() { return sqrt((this->vx) * (this->vx) + (this->vy) * (this->vy)); } //вычмсляет полную скорость частицы


	sf::CircleShape GetCircle() { return this->circle; } //нужна только для отрисовки круга

	void rebound() {
		//когда частица врезается в стену/потолок, она отскакивает, теряя часть энергии
		// Проверка выхода за границы окна
		if (this->GetY() - boundY * 0.9 > -5) { //сила трения о пол
			this->SetVx(this->GetVx() * 0.95);
			if (abs(this->GetVx()) <= 0.25) this->SetVx(0); //если скорость слишком мала, то остановка
		}
		if (this->GetX() > boundX * 0.9) {
			this->SetX(boundX * 0.9);
			this->SetVx(-this->GetVx() * 0.7);
		}
		else if (this->GetX() < boundX * 0.1) {
			this->SetX(boundX * 0.1);
			this->SetVx(-this->GetVx() * 0.7);
		}
		if (this->GetY() > boundY * 0.9) {
			this->SetY(boundY * 0.9);
			this->SetVy(-this->GetVy() * 0.7);
			if (abs(this->GetVy()) <= 0.25) this->SetVy(0); //если скорость слишком мала, то остановка
		}
	}

	void recolour() {
		//цвет частицы зависит от ее скорости
		int red;
		int green;
		int blue;
		if ((int)this->Find_speed() * 8 > 127) { //если скорость слишком большая, то цвет больше не меняем
			red = 100;
			green = 255;
			blue = 127;
			return;
		}
		red = 100;
		green = 128 + (int)(this->Find_speed() * 8);
		blue = 240 - (int)(this->Find_speed() * 8);
		sf::Color circleColor(red, green, blue);
		this->circle.setFillColor(circleColor);
	}


};



void Molecular_Interaction(Particle A, Particle B) { //взаимодействие между частицами
	//ТУТ ПОТОМ БУДЕТ КАРТА ГРАДИЕНТА И ХЭШИРОВАНИЕ
	//============ ДОПИСАТЬ ===============//
	std::cout << "Взаимодействие между частицами работает!" << std::endl;
}


int main()
{
	setlocale(LC_ALL, "Russian");
	sf::RenderWindow window(sf::VideoMode(boundX, boundY), "Fluid simulation");

	sf::Color wallColor(0, 0, 0);
	sf::Color backgroundColor(15, 15, 15);

	sf::RectangleShape rectangle1(sf::Vector2f(5, boundY * 0.9 + 25)); // Создаем прямоугольник 
	rectangle1.setFillColor(wallColor); // Устанавливаем цвет заливки
	rectangle1.setPosition(boundX * 0.1 - 25, 0); // Устанавливаем координаты прямоугольника

	sf::RectangleShape rectangle2(sf::Vector2f(5, boundY * 0.9 + 25)); // Создаем прямоугольник 
	rectangle2.setFillColor(wallColor); // Устанавливаем цвет заливки
	rectangle2.setPosition(boundX * 0.9 + 30, 0); // Устанавливаем координаты прямоугольника

	sf::RectangleShape rectangle3(sf::Vector2f(boundX * 0.8 + 60, 5)); // Создаем прямоугольник 
	rectangle3.setFillColor(wallColor); // Устанавливаем цвет заливки
	rectangle3.setPosition(boundX * 0.1 - 25, boundY * 0.9 + 25); // Устанавливаем координаты прямоугольника


	Particle particle_1(600, 400); //создаем одну частицу с кооординатами и скоростями
	particle_1.SetVx(15);
	particle_1.SetVy(-18);


	/*int x_number_of_particels; //определяет размер массива (прямоугольника), заполненного частицами
	int y_number_of_particels;
	std::cout << "Введите размеры прямоугольника частиц: " << std::endl;
	std::cin >> x_number_of_particels >> y_number_of_particels;

	Particle** ptr_for_particles_arrays = new Particle*[x_number_of_particels]; //создаем двумерный массив для частиц
	for (int i = 0; i < x_number_of_particels; i++) {
		ptr_for_particles_arrays[i] = new Particle[y_number_of_particels];
	}*/
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		particle_1.rebound(); //частица отталкивается
		particle_1.Earth_Gravity(); //притягивается к земле 1 раз
		particle_1.recolour(); //меняет цвет
		particle_1.move(); //движется
		particle_1.Earth_Gravity(); //притягивается к земле 2 раз
		sleep(50);

		window.clear(backgroundColor);
		window.draw(particle_1.GetCircle());
		window.draw(rectangle1);
		window.draw(rectangle2);
		window.draw(rectangle3);
		window.display();
	}

	return 0;
}

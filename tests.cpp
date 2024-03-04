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
const double PI = 3.14159; //число пи 
const double g = 9.80665; //ускорение свободного падения
//const int coef = 1; //коэф пропорциональности между силой отталкивания и массами частиц деленными на расстояние между ними
const int boundX = 1200; //размеры открываемого окна в sfml
const int boundY = 800; //размеры открываемого окна в sfml
const float Radius_of_Interaction = 1; //радиус области взаимодействия данной частицы с остальными



long double Find_Distance(double x1, double y1, double x2, double y2) { //ищет расстояние между центрами двух частиц
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

class Particle
//класс самого градиента "молекулы" и ее отображения в виде частицы
//умеет притягиваться к земле, отталкиваться от других частиц и обладает вязкостью
//обитает в двумерном простратстве (пока что)
{

private:
	double x; //координаты
	double y;
	double vx = 0; //скорости
	double vy = 0;
	const int mass = 1; //масса, сосредоточенная в центре
	const double r = 5; //радиус отображаемой частицы

	//===================================================================================//
	const float a = 1; //смещение 1/x^2 по осям x и у в методе Gradient_of_Interaction
	const float alpha = 1; //коэф растяжения 1/x^2 в методе Gradient_of_Interaction
	//===================================================================================//

public:
	Particle(float x, float y) { //создает частицу с коорд х::у
		this->x = x;
		this->y = y;
		std::cout << "частица с координатами" << x << "::" << y << "создалась" << std::endl;
	}
	~Particle() { std::cout << "частица с координатами" << x << "::" << y << "низведена до атомов" << std::endl; } //уничтожает частицу с коорд х::у


	double GetX() { return this->x; } //геттеры для коорд
	double GetY() { return this->y; }
	void SetCoord(double x, double y) { this->x = x; this->y = y; } //сеттер для коорд (хз зачем)

	void Earth_Gravity() {
		//гравитация, притягивает частицу вниз
		this->vy -= g;
	}

	double Gradient_of_Interaction(Particle A) {
		//рассчитывает величину "поля" данной частицы в точке другой частицы A с коорд x::y
		//возвращает 0, если частица вне радиуса взаимодействия, величину "градиента" в противном случае
		//величина "градиента" зависит от координаты как 1/x^2, причем (не)гипербола смещена вправо на константу a по оси х, вниз на а по оси у и растянута в альфа раз
		//!АЛЬФУ В ФОРМУЛЕ НУЖНО ПОМЕНЯТЬ НА НЕОБХОДИМОЕ ЗНАЧЕНИЕ(СЕЙЧАС ОНА ЕДИНИЦА), ОТ НЕЕ ЗАВИСЯТ СВОЙСТВА ЖИДКОСТИ
		//ro - расстояние от данной частицы до частицы А
		double ro = Find_Distance(A.x, A.y, this->x, this->y);
		if (ro >= Radius_of_Interaction) return 0;
		return (alpha / ((ro + a) * (ro + a)) - a);
	}


};


void Molecular_Interaction(Particle A, Particle B) { //взаимодействие между частицами
	//ТУТ ПОТОМ БУДЕТ КАРТА ГРАДИЕНТА И ХЭШИРОВАНИЕ
	//============ ДОПИСАТЬ ===============//
	std::cout << "Взаимодействие между частицами работает!" << std::endl;
}


void sleep(int ms)
//задержка на ms миллисекунд
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}



int main()
{
	sf::RenderWindow window(sf::VideoMode(boundX, boundY), "SFML Moving Circle Example");

	sf::CircleShape circle(5); // радиус круга
	circle.setFillColor(sf::Color::Red); // цвет круга
	circle.setPosition(400, 300); // начальная позиция круга

	float speedX = 0; // скорость движения по оси X
	float speedY = 5; // скорость движения по оси Y

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Обновление позиции круга
		sf::Vector2f circlePos = circle.getPosition();
		circle.setPosition(circlePos.x + speedX, circlePos.y + speedY);
		speedY += 1;
		// Проверка выхода за границы окна
		if (circlePos.x < 0 || circlePos.x > 800)
			speedX = -1;
		if (circlePos.y < 0 || circlePos.y > 500) {
			circle.setPosition(circlePos.x, 500);
			speedY = -speedY * 0.6;
			if (abs(speedY) <= 3) speedY -= 1;
		}
		else sleep(25);
		

		window.clear();
		window.draw(circle);
		window.display();
	}

	return 0;
}


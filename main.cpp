#include <SFML/Graphics.hpp>
#include <cmath>
#include <chrono>
#include <thread>
#include <vector>
#include <set>
#include <iostream>

using namespace sf;

//================================================//
//  !!!����� ������ ���������������� ��� �OUT!!!
//================================================//



//�������� ���� �������� double �� float, ����� ���� �������
const double PI = 3.14; //����� �� 
const double g = 0.10; //��������� ���������� �������
const int coef = 1; //���� ������������������ ����� ����� ������������ � ������� ������ ��������� �� ���������� ����� ����
const int boundX = 1200; //������� ������������ ���� � sfml
const int boundY = 800; //������� ������������ ���� � sfml
const float Radius_of_Interaction = 1; //������ ������� �������������� ������ ������� � ����������



long double Find_Distance(double x1, double y1, double x2, double y2) { //���� ���������� ����� �������� ���� ������
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void sleep(int ms) {
	//�������� �� ms �����������
	std::this_thread::sleep_for(std::chrono::microseconds(ms));
}


class Particle
	//����� ������ ��������� "��������" � �� ����������� � ���� �������
	//����� ������������� � �����, ������������� �� ������ ������ � �������� ���������
	//������� � ��������� ������������ (���� ���)
{

private:
	sf::CircleShape circle; //������ ������� ������������ ������ 
	const double r = 10; //������ ������������ �������
	double vx = 0; //��������
	double vy = 0;
	const int mass = 1; //�����, ��������������� � ������

	//===================================================================================//
	const float a = 1; //�������� 1/x^2 �� ���� x � � � ������ Gradient_of_Interaction
	const float alpha = 1; //���� ���������� 1/x^2 � ������ Gradient_of_Interaction
	//===================================================================================//

public:
	Particle() { //������� ������� ������� � ����� 0::0
		sf::Color circleColor(100, 255, 127);
		circle.setFillColor(circleColor);
		circle.setPosition(0, 0);
		circle.setRadius(r);
		std::cout << "������� ������� � ������������" << 0 << "::" << 0 << "���������" << std::endl;
	}
	Particle(float x_, float y_) { //������� ������� � ����� �::�
		sf::Color circleColor(100, 255, 127);
		circle.setFillColor(circleColor);
		circle.setPosition(x_, y_);
		circle.setRadius(r);
		std::cout << "������� ������� � ������������" << x_ << "::" << y_ << "���������" << std::endl;
	}

	double GetX() { return this->circle.getPosition().x; } //������� � ������� ��� �����
	double GetY() { return this->circle.getPosition().y; } //�� ���� ��� ��� �������� � sfml, �� ��� ��� ���������� ������� �������(
	void SetX(double x_) { double x = GetX(); double y = GetY(); this->circle.setPosition(x_, y); }
	void SetY(double y_) { double x = GetX(); double y = GetY(); this->circle.setPosition(x, y_); }

	double GetVx() { return this->vx; } //������� � ������� ��� ���������
	double GetVy() { return this->vy; }
	void SetVx(double vx_) { this->vx = vx_; }
	void SetVy(double vy_) { this->vy = vy_; }

	void Earth_Gravity() { this->vy += g; } //����������, ����������� ������� ����

	double Gradient_of_Interaction(Particle A) {
		//������������ �������� "����" ������ ������� � ����� ������ ������� A � ����� x::y
		//���������� 0, ���� ������� ��� ������� ��������������, �������� "���������" � ��������� ������
		//�������� "���������" ������� �� ���������� ��� 1/x^2, ������ ��������� ������� ������ �� ��������� a �� ��� �, ���� �� � �� ��� � � ��������� � ����� ���
		//!����� � ������� ����� �������� �� ����������� ��������(������ ��� �������), �� ��� ������� �������� ��������
		//ro - ���������� �� ������ ������� �� ������� �
		double ro = Find_Distance(A.GetX(), A.GetY(), this->GetX(), this->GetY());
		if (ro >= Radius_of_Interaction) return 0;
		return (alpha / ((ro + a) * (ro + a)) - a);
	}

	void move() {  //�������� �������
		double x = GetX();
		double y = GetY();
		this->circle.setPosition(x += vx, y += vy); 
	}


	long double Find_speed() { return sqrt((this->vx) * (this->vx) + (this->vy) * (this->vy)); } //��������� ������ �������� �������


	sf::CircleShape GetCircle() { return this->circle; } //����� ������ ��� ��������� �����

	void rebound() {
		//����� ������� ��������� � �����/�������, ��� �����������, ����� ����� �������
		// �������� ������ �� ������� ����
		if (this->GetY() - boundY * 0.9 > -5) { //���� ������ � ���
			this->SetVx(this->GetVx() * 0.95);
			if (abs(this->GetVx()) <= 0.25) this->SetVx(0); //���� �������� ������� ����, �� ���������
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
			if (abs(this->GetVy()) <= 0.25) this->SetVy(0); //���� �������� ������� ����, �� ���������
		}
	}

	void recolour() {
		//���� ������� ������� �� �� ��������
		int red;
		int green;
		int blue;
		if ((int)this->Find_speed() * 8 > 127) { //���� �������� ������� �������, �� ���� ������ �� ������
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



void Molecular_Interaction(Particle A, Particle B) { //�������������� ����� ���������
	//��� ����� ����� ����� ��������� � �����������
	//============ �������� ===============//
	std::cout << "�������������� ����� ��������� ��������!" << std::endl;
}


int main()
{
	setlocale(LC_ALL, "Russian");
	sf::RenderWindow window(sf::VideoMode(boundX, boundY), "Fluid simulation");

	sf::Color wallColor(0, 0, 0);
	sf::Color backgroundColor(15, 15, 15);

	sf::RectangleShape rectangle1(sf::Vector2f(5, boundY * 0.9 + 25)); // ������� ������������� 
	rectangle1.setFillColor(wallColor); // ������������� ���� �������
	rectangle1.setPosition(boundX * 0.1 - 25, 0); // ������������� ���������� ��������������

	sf::RectangleShape rectangle2(sf::Vector2f(5, boundY * 0.9 + 25)); // ������� ������������� 
	rectangle2.setFillColor(wallColor); // ������������� ���� �������
	rectangle2.setPosition(boundX * 0.9 + 30, 0); // ������������� ���������� ��������������

	sf::RectangleShape rectangle3(sf::Vector2f(boundX * 0.8 + 60, 5)); // ������� ������������� 
	rectangle3.setFillColor(wallColor); // ������������� ���� �������
	rectangle3.setPosition(boundX * 0.1 - 25, boundY * 0.9 + 25); // ������������� ���������� ��������������


	Particle particle_1(600, 400); //������� ���� ������� � ������������� � ����������
	particle_1.SetVx(15);
	particle_1.SetVy(-18);


	/*int x_number_of_particels; //���������� ������ ������� (��������������), ������������ ���������
	int y_number_of_particels;
	std::cout << "������� ������� �������������� ������: " << std::endl;
	std::cin >> x_number_of_particels >> y_number_of_particels;

	Particle** ptr_for_particles_arrays = new Particle*[x_number_of_particels]; //������� ��������� ������ ��� ������
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

		particle_1.rebound(); //������� �������������
		particle_1.Earth_Gravity(); //������������� � ����� 1 ���
		particle_1.recolour(); //������ ����
		particle_1.move(); //��������
		particle_1.Earth_Gravity(); //������������� � ����� 2 ���
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

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
const double g = 1; //��������� ���������� �������
const int coef = 1; //���� ������������������ ����� ����� ������������ � ������� ������ ��������� �� ���������� ����� ����
const int boundX = 1200; //������� ������������ ���� � sfml
const int boundY = 800; //������� ������������ ���� � sfml
const float Radius_of_Interaction = 1; //������ ������� �������������� ������ ������� � ����������



long double Find_Distance(double x1, double y1, double x2, double y2) { //���� ���������� ����� �������� ���� ������
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void sleep(int ms) {
	//�������� �� ms �����������
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
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
		sf::Color circleColor(0, 128, 255);
		circle.setFillColor(circleColor);
		circle.setPosition(0, 0);
		circle.setRadius(r);
		std::cout << "������� ������� � ������������" << 0 << "::" << 0 << "���������" << std::endl;
	}
	Particle(float x_, float y_) { //������� ������� � ����� �::�
		sf::Color circleColor(0, 128, 255);
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

	void set_color(int r, int g, int b) { //������ ���� �����
		sf::Color circleColor(r, g, b);
		this->circle.setFillColor(circleColor);
	}

	sf::CircleShape GetCircle() { return this->circle; } //����� ������ ��� ��������� �����


};



void Molecular_Interaction(Particle A, Particle B) { //�������������� ����� ���������
	//��� ����� ����� ����� ��������� � �����������
	//============ �������� ===============//
	std::cout << "�������������� ����� ��������� ��������!" << std::endl;
}

void rebound(Particle &A) {
	//����� ������� ��������� � �����/�������, ��� �����������, ����� ����� �������
	// �������� ������ �� ������� ����
	if (A.GetY() - boundY * 0.9 > -5) { //���� ������ � ���
		A.SetVx(A.GetVx() * 0.95);
		if (abs(A.GetVx()) <= 0.5) A.SetVx(0); //���� �������� ������� ����, �� ���������
	}
	if (A.GetX() > boundX * 0.9) {
		A.SetX(boundX * 0.9);
		A.SetVx(- A.GetVx() * 0.7);
	}
	else if (A.GetX() < boundX * 0.1) {
		A.SetX(boundX * 0.1);
		A.SetVx(-A.GetVx() * 0.7);
	}
	if (A.GetY() > boundY * 0.9) {
		A.SetY(boundY * 0.9);
		A.SetVy(-A.GetVy() * 0.7);
		if (abs(A.GetVy()) <= 0.5) A.SetVy(0); //���� �������� ������� ����, �� ���������
	}
}

void recolor(Particle &A) {
	//���� ������� ������� �� �� ��������
	if ((int)A.Find_speed() * 3 > 127) { //���� �������� ������� �������, �� ���� ������ �� ������
		A.set_color(0, 255, 0);
		return;
	}
	A.set_color(0, 128 + (int)A.Find_speed() * 3, 255 - (int)A.Find_speed() * 3);
}




int main()
{
	setlocale(LC_ALL, "Russian");
	sf::RenderWindow window(sf::VideoMode(boundX, boundY), "Fluid simulation");

	Particle particle_1(600, 400); //������� ���� ������� � ������������� � ����������
	particle_1.SetVx(30);
	particle_1.SetVy(-10);

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

		rebound(particle_1); //������� �������������
		recolor(particle_1); //������ ����
		particle_1.move(); //��������
		particle_1.Earth_Gravity(); //������������� � �����
		sleep(25);

		window.clear();
		window.draw(particle_1.GetCircle());
		window.display();
	}

	return 0;
}

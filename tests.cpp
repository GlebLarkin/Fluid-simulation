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
const double PI = 3.14159; //����� �� 
const double g = 9.80665; //��������� ���������� �������
//const int coef = 1; //���� ������������������ ����� ����� ������������ � ������� ������ ��������� �� ���������� ����� ����
const int boundX = 1200; //������� ������������ ���� � sfml
const int boundY = 800; //������� ������������ ���� � sfml
const float Radius_of_Interaction = 1; //������ ������� �������������� ������ ������� � ����������



long double Find_Distance(double x1, double y1, double x2, double y2) { //���� ���������� ����� �������� ���� ������
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

class Particle
//����� ������ ��������� "��������" � �� ����������� � ���� �������
//����� ������������� � �����, ������������� �� ������ ������ � �������� ���������
//������� � ��������� ������������ (���� ���)
{

private:
	double x; //����������
	double y;
	double vx = 0; //��������
	double vy = 0;
	const int mass = 1; //�����, ��������������� � ������
	const double r = 5; //������ ������������ �������

	//===================================================================================//
	const float a = 1; //�������� 1/x^2 �� ���� x � � � ������ Gradient_of_Interaction
	const float alpha = 1; //���� ���������� 1/x^2 � ������ Gradient_of_Interaction
	//===================================================================================//

public:
	Particle(float x, float y) { //������� ������� � ����� �::�
		this->x = x;
		this->y = y;
		std::cout << "������� � ������������" << x << "::" << y << "���������" << std::endl;
	}
	~Particle() { std::cout << "������� � ������������" << x << "::" << y << "��������� �� ������" << std::endl; } //���������� ������� � ����� �::�


	double GetX() { return this->x; } //������� ��� �����
	double GetY() { return this->y; }
	void SetCoord(double x, double y) { this->x = x; this->y = y; } //������ ��� ����� (�� �����)

	void Earth_Gravity() {
		//����������, ����������� ������� ����
		this->vy -= g;
	}

	double Gradient_of_Interaction(Particle A) {
		//������������ �������� "����" ������ ������� � ����� ������ ������� A � ����� x::y
		//���������� 0, ���� ������� ��� ������� ��������������, �������� "���������" � ��������� ������
		//�������� "���������" ������� �� ���������� ��� 1/x^2, ������ (��)��������� ������� ������ �� ��������� a �� ��� �, ���� �� � �� ��� � � ��������� � ����� ���
		//!����� � ������� ����� �������� �� ����������� ��������(������ ��� �������), �� ��� ������� �������� ��������
		//ro - ���������� �� ������ ������� �� ������� �
		double ro = Find_Distance(A.x, A.y, this->x, this->y);
		if (ro >= Radius_of_Interaction) return 0;
		return (alpha / ((ro + a) * (ro + a)) - a);
	}


};


void Molecular_Interaction(Particle A, Particle B) { //�������������� ����� ���������
	//��� ����� ����� ����� ��������� � �����������
	//============ �������� ===============//
	std::cout << "�������������� ����� ��������� ��������!" << std::endl;
}


void sleep(int ms)
//�������� �� ms �����������
{
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}



int main()
{
	sf::RenderWindow window(sf::VideoMode(boundX, boundY), "SFML Moving Circle Example");

	sf::CircleShape circle(5); // ������ �����
	circle.setFillColor(sf::Color::Red); // ���� �����
	circle.setPosition(400, 300); // ��������� ������� �����

	float speedX = 0; // �������� �������� �� ��� X
	float speedY = 5; // �������� �������� �� ��� Y

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// ���������� ������� �����
		sf::Vector2f circlePos = circle.getPosition();
		circle.setPosition(circlePos.x + speedX, circlePos.y + speedY);
		speedY += 1;
		// �������� ������ �� ������� ����
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


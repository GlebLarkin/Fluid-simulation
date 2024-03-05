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

void sleep(int mc) {
	//�������� �� ms �����������
	std::this_thread::sleep_for(std::chrono::microseconds(mc));
}


class Particle
	//����� ������ ��������� "��������" � �� ����������� � ���� �������
	//����� ������������� � �����, ������������� �� ������ ������ � �������� ���������
	//������� � ��������� ������������ (���� ���)
{

private:
	sf::CircleShape circle; //������ ������� ������������ ������ 
	const double r = 8; //������ ������������ �������
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
		//!����� � ������� ����� ��������
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
		//����� ������� ��������� � �����/���, ��� �����������, ����� ����� �������
		if (this->GetY() - boundY + 15 > -5) { //���� ������ � ���
			this->SetVx(this->GetVx() * 0.97);
			if (abs(this->GetVx()) <= 0.1) this->SetVx(0); //���� �������� ������� ����, �� ���������
		}
		if (this->GetX() > boundX - 15) {
			this->SetX(boundX - 15);
			this->SetVx(-this->GetVx() * 0.7);
		}
		else if (this->GetX() < 0) {
			this->SetX(0);
			this->SetVx(-this->GetVx() * 0.7);
		}
		if (this->GetY() > boundY - 17) {
			this->SetY(boundY - 17);
			this->SetVy(-this->GetVy() * 0.7);
			if (abs(this->GetVy()) <= 0.2) this->SetVy(0); //���� �������� ������� ����, �� ���������
		}
	}

	void recolour() {
		//���� ������� ������� �� �� ��������
		int red;
		int green;
		int blue;
		if ((int)this->Find_speed() * 6 > 127) { //���� �������� ������� �������, �� ���� ������ �� ������
			red = 100;
			green = 255;
			blue = 127;
			return;
		}
		red = 100;
		green = 120 + (int)(this->Find_speed() * 6);
		blue = 240 - (int)(this->Find_speed() * 6);
		sf::Color circleColor(red, green, blue);
		this->circle.setFillColor(circleColor);
	}


};



void Molecular_Interaction(Particle A, Particle B) { //�������������� ����� ���������
	//��� ����� ����� ����� ��������� � �����������
	//============ �������� ===============//
	std::cout << "�������������� ����� ��������� ��������!" << std::endl;
}

void left_mouse_click(Particle &A, RenderWindow* window_ptr) {
	//��������� ���������� � ������� ��� ������� ����
	//���� ��� ��������: � ������������ �� ����� � �� ����� � ��������
	//�������� ������, ��� ��� ����� ������ ����������������� � �������� ���������
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window_ptr);
	sf::Vector2f direction = sf::Vector2f(mousePosition) - A.GetCircle().getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	//direction /= (length / 0.4);
	direction = (Vector2f)(direction * 100.0f / (length * length));

	double vx = A.GetVx();
	double vy = A.GetVy();

	vx += direction.x;
	vy += direction.y;
	if (length < 40) return; //��� ����������� �� �������� ����� ������� �� ������ ������������ � ������� ������� ������
	A.SetVx(vx);
	A.SetVy(vy);
}

void right_mouse_click(Particle& A, RenderWindow* window_ptr) {
	//��������� ������������ �� ������� ��� ������� ����
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*window_ptr);
	sf::Vector2f direction = sf::Vector2f(mousePosition) - A.GetCircle().getPosition();
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	direction = (Vector2f)(direction * 100.0f / (length * length));

	double vx = A.GetVx();
	double vy = A.GetVy();

	vx -= 0.3 * direction.x;
	vy -= 0.3 * direction.y;

	A.SetVx(vx);
	A.SetVy(vy);
}



int main()
{
	setlocale(LC_ALL, "Russian");
	sf::RenderWindow window(sf::VideoMode(boundX, boundY), "Fluid simulation");

	Particle particle_1(600, 400); //������� ���� ������� � ������������� � ����������


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

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) left_mouse_click(particle_1, &window); //���������� � ������e ��� ������� ���
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) right_mouse_click(particle_1, &window); //������������ �� ������� ��� ������� ���

		sleep(50);
		window.clear();
		window.draw(particle_1.GetCircle());
		window.display();
	}

	return 0;
}

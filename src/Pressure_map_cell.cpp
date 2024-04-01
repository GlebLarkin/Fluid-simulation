#include <Pressure_map_cell.hpp>


sf::Vector2f Pressure_map_cell::GetCoord() { return this->coord; }
double Pressure_map_cell::GetPressure() { return this->pressure; }

// Setter methods
void Pressure_map_cell::SetCoord(float x_, float y_) { this->coord.x = x_; this->coord.y = y_; }
void Pressure_map_cell::SetPressure(double pressure_) { this->pressure = pressure_; }

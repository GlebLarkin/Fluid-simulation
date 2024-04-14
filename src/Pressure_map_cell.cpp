#include <Pressure_map_cell.hpp>


//sf::Vector2f Pressure_map_cell::GetCoord() { return this->coord; }
unsigned int Pressure_map_cell::GetCoord_x() {return this->coord_x; }
unsigned int Pressure_map_cell::GetCoord_y() {return this->coord_y; }
double Pressure_map_cell::GetPressure() { return this->pressure; }

// Setter methods
void Pressure_map_cell::SetCoord(float x_, float y_) { this->coord_x = x_; this->coord_y = y_; }
void Pressure_map_cell::SetPressure(double pressure_) { this->pressure = pressure_; }

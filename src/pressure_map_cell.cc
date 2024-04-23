#include <pressure_map_cell.h>


//sf::Vector2f Pressure_map_cell::GetCoord() { return this->coord; }
unsigned int PressureMapCell::GetCoordX() {return this->coord_x_; }
unsigned int PressureMapCell::GetCoordY() {return this->coord_y_; }
double PressureMapCell::GetPressure() { return this->pressure_; }

// Setter methods
void PressureMapCell::SetCoord(float x, float y) { this->coord_x_ = x; this->coord_y_ = y; }
void PressureMapCell::SetPressure(double pressure) { this->pressure_ = pressure; }

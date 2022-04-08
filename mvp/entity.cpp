#include "entity.h"

Entity::Entity() {
    x_position = 0;
    y_position = 0;
    radius = 0;
    speed_X = 0;
    speed_Y = 0;
    color = QColor(rand() % 255, rand() % 255, rand() % 255);
}

double Entity::get_x_position() const {
    return x_position;
}

double Entity::get_y_position() const {
    return y_position;
}

double Entity::get_radius() const {
    return radius;
}

Entity::Entity(double r_) {
    x_position = 250.0 + rand() % 1400;
    y_position = 100.0 + rand() % 700;

    speed_X = 0;
    speed_Y = 0;

    color  = QColor(rand() % 255, rand() % 255, rand() % 255);
    radius = r_;
}

Entity::Entity(double r_, QColor color_) {
    x_position = 250.0 + rand() % 1400;
    y_position = 100.0 + rand() % 700;

    speed_X = 0;
    speed_Y = 0;

    color = color_;
    radius = r_;
}


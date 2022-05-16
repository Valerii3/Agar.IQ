#include "../Include/entity.h"

Entity::Entity() {
    x_coordinate = 0;
    y_coordinate = 0;
    radius = 0;
    color = QColor(rand() % 255, rand() % 255, rand() % 255);
}

double Entity::get_x_position() const {
    return x_coordinate;
}

double Entity::get_y_position() const {
    return y_coordinate;
}

double Entity::get_radius() const {
    return radius;
}

Entity::Entity(double r_) {
    x_coordinate = 250.0 + rand() % 1400;
    y_coordinate = 100.0 + rand() % 700;

    color  = QColor(rand() % 255, rand() % 255, rand() % 255);
    radius = r_;
}

Entity::Entity(double r_, QColor color_) {
    x_coordinate = 250.0 + rand() % 1400;
    y_coordinate = 100.0 + rand() % 700;

    color = color_;
    radius = r_;
}

Entity::Entity(double x, double y, double r_) {
    x_coordinate = x;
    y_coordinate = y;
    radius = r_;

    color = QColor(rand() % 255, rand() % 255, rand() % 255);
}

Entity::Entity(double x, double y, double r_, QColor color_) {
    x_coordinate = x;
    y_coordinate = y;

    color = color_;
    radius = r_;
}


#include "entity.h"

double Entity::get_x_position() const {
    return x_coordinate;
}

double Entity::get_y_position() const {
    return y_coordinate;
}

double Entity::get_radius() const {
    return radius;
}

Entity::Entity(double r) {
    x_coordinate = -500.0 + rand() % 3000;
    y_coordinate = -300.0 + rand() % 1500;

    radius = r;

    red_color = rand() % 255;
    green_color = rand() % 255;
    blue_color = rand() % 255;
}

Entity::Entity(double x, double y, double r) {
    x_coordinate = x;
    y_coordinate = y;
    radius = r;

    red_color = rand() % 255;
    green_color = rand() % 255;
    blue_color = rand() % 255;
}

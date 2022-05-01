#include "entity.h"

Entity::Entity() {
    x_coordinate = 0;
    y_coordinate = 0;
    radius = 0;
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

    radius = r_;
}

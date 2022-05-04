#include "entity.h"

Entity::Entity(int id) {
    x_coordinate = 0;
    y_coordinate = 0;
    radius = 0;

    entity_id = id;
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

Entity::Entity(double r_, int id) {
    x_coordinate = 250.0 + rand() % 1400;
    y_coordinate = 100.0 + rand() % 700;

    radius = r_;
    entity_id = id;
}

Entity::Entity(double x, double y, double r, int id) {
    x_coordinate = x;
    y_coordinate = y;
    radius = r;

    entity_id = id;
}

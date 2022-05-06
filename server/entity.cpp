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

Entity::Entity(double r_, int id) {
    x_coordinate = 250.0 + rand() % 1400;
    y_coordinate = 100.0 + rand() % 700;

    radius = r_;
    entity_id = id;
}

Entity::Entity(double r) {
    x_coordinate = 250.0 + rand() % 1400;
    y_coordinate = 100.0 + rand() % 700;

    radius = r;

    red_color = rand() % 255;
    green_color = rand() % 255;
    blue_color = rand() % 255;
}

Entity::Entity(double x, double y, double r, int id) {
    x_coordinate = x;
    y_coordinate = y;
    radius = r;

    entity_id = id;

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

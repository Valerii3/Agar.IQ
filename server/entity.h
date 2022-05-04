#ifndef ENTITY_H
#define ENTITY_H

#include <QString>

struct Entity {
    double x_coordinate;
    double y_coordinate;
    double radius;

    QString type = "entity";

    int entity_id;

    double get_x_position() const;
    double get_y_position() const;
    double get_radius() const;

    Entity() = default;

    Entity(int id);

    Entity(double r_, int id);

    Entity(double x, double y, double r, int id);
};

#endif // ENTITY_H

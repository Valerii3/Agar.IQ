#ifndef ENTITY_H
#define ENTITY_H

#include <QString>

struct Entity {
    double x_coordinate;
    double y_coordinate;
    double radius;

    QString type = "entity";

    double get_x_position() const;
    double get_y_position() const;
    double get_radius() const;

    Entity();

    Entity(double r_);
};

#endif // ENTITY_H

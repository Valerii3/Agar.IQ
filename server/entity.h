#ifndef ENTITY_H
#define ENTITY_H

#include <QString>
#include <QRandomGenerator>

struct Entity {
    double x_coordinate;
    double y_coordinate;
    double radius;

    QString type = "entity";

    int red_color = 0;
    int green_color = 0;
    int blue_color = 0;

    double get_x_position() const;
    double get_y_position() const;
    double get_radius() const;

    Entity() = default;

    Entity(double r);

    Entity(double x, double y, double r);
};

#endif // ENTITY_H

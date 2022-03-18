#ifndef ENTITY_H
#define ENTITY_H

#include <QColor>
#include <QRandomGenerator>

struct Entity {
    double x_position;
    double y_position;
    double radius;

    double speed_X;
    double speed_Y;
    QColor color;

    double get_x_position() const;
    double get_y_position() const;
    double get_radius() const;

    Entity();

    Entity(double r_);

    Entity(double r_, QColor color_);
};

#endif // ENTITY_H
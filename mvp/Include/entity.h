#ifndef ENTITY_H
#define ENTITY_H

#include <QColor>
#include <QRandomGenerator>

struct Entity {
    double x_position;
    double y_position;
    double radius;

    std::string type = "entity";


    QColor color;

    double get_x_position() const;
    double get_y_position() const;
    double get_radius() const;

    Entity();

    Entity(double r_);

    Entity(double r_, QColor color_);

    Entity(double x, double y, double r_);

    Entity(double x, double y, double r_, QColor color_);
};

#endif // ENTITY_H

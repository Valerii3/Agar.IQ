#ifndef FOOD_H
#define FOOD_H

#include "entity.h"

struct Food : Entity {
public:
    Food();

    Food(double x, double y);

    Food(double x, double y, int red, int green, int blue);
};

#endif // FOOD_H

#ifndef FOOD_H
#define FOOD_H

#include "entity.h"

struct Food : Entity {
public:
    Food();

    Food(int i);

    Food(double x, double y);
};


#endif // FOOD_H

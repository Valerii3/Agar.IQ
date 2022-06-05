#include "food.h"

Food::Food() : Entity(5) {
    type = "food";
}

Food::Food(double x, double y) : Entity(x, y, 5) {
    type = "food";
}


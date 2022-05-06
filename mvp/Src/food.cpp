#include "../Include/food.h"

Food::Food() : Entity(5) {
    type = "food";
}

Food::Food(double x, double y) : Entity(x, y, 5) {
    type = "food";
}

Food::Food(double x, double y, int red, int green, int blue) :
    Entity(x, y, 5, QColor(red, green, blue)) {
    type = "food";
}

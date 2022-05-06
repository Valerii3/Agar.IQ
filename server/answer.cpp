#include "answer.h"

Answer::Answer(double x, double y, int number_) : Entity(15) {
    number = number_;
    type = "answer";

    red_color = 254;
    green_color = 0;
    blue_color = 0;
}

int Answer::get_number() const {
    return number;
}

Answer::Answer(int i) : Entity(15, i) {
    number = 0;
    type = "answer";

    red_color = 254;
    green_color = 0;
    blue_color = 0;
}

Answer::Answer(double x, double y) : Entity(x, y, 15) {
    number = 0;
    type = "answer";

    red_color = 254;
    green_color = 0;
    blue_color = 0;
}

void Answer::update_number(int number_) {
    number = number_;
}

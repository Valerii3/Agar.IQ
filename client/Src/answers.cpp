#include "../Include/answers.h"

Answer::Answer(int number_) : Entity(15, Qt::red) {
    number = number_;
    type = "answer";
}

int Answer::get_number() const {
    return number;
}

Answer::Answer(double x, double y) : Entity(x, y, 15, Qt::red) {
    number = 0;
    type = "answer";
}

void Answer::update_number(int number_) {
    number = number_;
}

Answer::Answer(double x, double y, int number_) : Entity(x, y, 15, Qt::red) {
    number = number_;
    type = "answer";
}

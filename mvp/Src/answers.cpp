#include "../Include/answers.h"

Answer::Answer(int n_) : Entity(15, Qt::red) {
    n = n_;
    type = "answer";
}

int Answer::get_number() const {
    return n;
}

Answer::Answer(double x, double y) : Entity(x, y, 15, Qt::red){
    type = "answer";
}

void Answer::update_number(int n_) {
    n = n_;
}

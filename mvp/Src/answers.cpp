#include "../Include/answers.h"

Answer::Answer(int n_) : Entity(15, Qt::red) {
    n = n_;
    type = "answer";
}

int Answer::get_number() const {
    return n;
}

#include "answers.h"

Answer::Answer(int n_) : Entity(15, Qt::red) {
    n = n_;
}

int Answer::get_number() {
    return n;
}

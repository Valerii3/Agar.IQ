#ifndef ANSWERS_H
#define ANSWERS_H

#include "entity.h"

struct Answer : Entity {
    int n;
public:
    Answer(int n_);
    int get_number();
};

#endif // ANSWERS_H

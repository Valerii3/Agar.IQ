#ifndef ANSWERS_H
#define ANSWERS_H

#include "entity.h"

struct Answer : Entity {
    int n;
public:
    Answer(int n_);

    Answer(double x, double y);

    int get_number() const;

    void update_number(int n_);
};

#endif // ANSWERS_H

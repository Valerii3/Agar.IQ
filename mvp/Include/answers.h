#ifndef ANSWERS_H
#define ANSWERS_H

#include "entity.h"

struct Answer : Entity {
    int number;
public:
    Answer(int number_);

    Answer(double x, double y);

    int get_number() const;

    void update_number(int number_);
};

#endif // ANSWERS_H

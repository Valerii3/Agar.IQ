#ifndef ANSWER_H
#define ANSWER_H

#include "entity.h"

struct Answer : Entity
{
public:
    int number;

    Answer() = default;

    Answer(int i);

    Answer(double x, double y, int number_);

    Answer(double x, double y);

    int get_number() const;

    void update_number(int number_);
};

#endif // ANSWER_H

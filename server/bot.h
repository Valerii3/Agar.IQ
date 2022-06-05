#ifndef BOT_H
#define BOT_H

#include "entity.h"

struct Bot : Entity {
public:
    Bot();

    void change_angle(double new_angle);

    int score = 0;
    double bot_angle = 0;
    double bot_speed = 3.5;
};

#endif // BOT_H

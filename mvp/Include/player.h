#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

struct Player : Entity {
public:
    std::string player_name;
    std::string get_name() const;
    double player_speed = 0;
    double player_angle = 0;

    Player();
    Player(std::string name);

    Player(QString name, double x, double y, double rad);
};

#endif // PLAYER_H

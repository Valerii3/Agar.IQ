#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

struct Player : Entity {
public:
    std::string name;
    std::string get_name() const;
    double speed = 0, angle = 0;

    Player();
    Player(std::string name_);
};

#endif // PLAYER_H
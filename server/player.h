#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

struct Player : Entity {
public:

    std::string name;

    Player(QString name, double x, double y, double rad);
};

#endif // PLAYER_H

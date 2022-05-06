#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

struct Player : Entity {
public:

    std::string player_name;

    Player(QString name, double x, double y, double rad);

    Player(QString name, double x, double y, double rad, int id);
};

#endif // PLAYER_H

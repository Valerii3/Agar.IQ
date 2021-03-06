#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

struct Player : Entity {
public:

    std::string player_name;

    int player_id = 0;

    bool is_online = true;
    bool is_eaten = false;

    std::string player_initialization = "yes";

    int score = 0;
    double player_angle = 0;
    double player_speed = 3.5;
    std::string is_correct = "";

    Player(QString name, double x, double y, double rad);

    Player(QString name, double x, double y, double rad, int id);

    void eaten();
};

#endif // PLAYER_H

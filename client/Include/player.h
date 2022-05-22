#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

struct Player : Entity {
public:
    std::string player_name;
    std::string get_name() const;
    double player_speed = 0;
    double player_angle = 0;

    int score = 0;
    bool is_eaten = 0;
    QString is_correct = "";

    Player();
    Player(std::string name);

    Player(QString name, double x, double y, double rad);

    Player(QString name, double x, double y, double rad, int score_, QString is_correct_, bool is_eaten_);
};

#endif // PLAYER_H

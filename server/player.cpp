#include "player.h"

Player::Player(QString name, double x, double y, double rad, int id) : Entity(x, y, rad) {
    player_name = name.toStdString();

    score = 0;

    player_id = id;
    type = "player";
}

Player::Player(QString name, double x, double y, double rad) : Entity(x, y, rad) {
    player_name = name.toStdString();

    score = 0;

    type = "player";
}

void Player::eaten() {
    is_eaten = true;
}

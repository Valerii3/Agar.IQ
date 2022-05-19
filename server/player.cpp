#include "player.h"

Player::Player(QString name, double x, double y, double rad, int id) : Entity(x, y, rad) {
    player_name = name.toStdString();

    player_id = id;
    type = "player";
}

Player::Player(QString name, double x, double y, double rad) : Entity(x, y, rad) {
    player_name = name.toStdString();

    type = "player";
}

void Player::eaten() {
    is_eaten = true;
}

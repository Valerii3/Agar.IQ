#include "../Include/player.h"

Player::Player(std::string name) : Player() {
    player_name = name;
}

Player::Player() : Entity(25) {
    player_name = "Player";
}

std::string Player::get_name() const {
    return player_name;
}

Player::Player(QString name, double x, double y, double rad) {
    player_name = name.toStdString();
    x_coordinate = x;
    y_coordinate = y;
    radius = rad;
}

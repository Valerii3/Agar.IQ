#include "../Include/player.h"

Player::Player(std::string name_) : Player() {
    name = name_;
}

Player::Player() : Entity(25) {
    name = "Илья";
}

std::string Player::get_name() const {
    return name;
}

Player::Player(QString name, double x, double y, double rad) {
    this->name = name.toStdString();
    x_position = x;
    y_position = y;
    radius = rad;
}

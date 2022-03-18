#include "player.h"

Player::Player(std::string name_) : Entity(25) {
    name = name_;
}

Player::Player() : Entity(25) {
    name = "hui";
}

std::string Player::get_name() const {
    return name;
}
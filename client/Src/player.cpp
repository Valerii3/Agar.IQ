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

Player::Player(QString name, double x, double y, double rad) : Entity(x, y, rad){
    player_name = name.toStdString();
}

Player::Player(QString name, double x, double y, double rad, int score_, QString is_correct_, bool is_eaten_) : Entity(x, y, rad){
    player_name = name.toStdString();    

    score = score_;
    is_correct = is_correct_;
    is_eaten = is_eaten_;
}

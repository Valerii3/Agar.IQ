#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

struct Player {
public:
    QString name;

    double x_coordinate;
    double y_coordinate;
    double radius;

    Player(QString name, double x, double y, double rad);
};

#endif // PLAYER_H

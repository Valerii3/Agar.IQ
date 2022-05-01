#ifndef SCENE_H
#define SCENE_H

#include <QVector>

#include "entity.h"
#include "player.h"

class scene
{
public:
    scene();

    QVector<Entity> answers;

    QVector<Entity> foods;

    QVector<Player> players;

    void new_food(int i);

    void new_answer(int i);

    void new_player(QString name);

    bool collision(Entity a, Entity b);

    void generate_answers();

    void generate_food();

    void update_player(int iter, QString name, double x, double y, double rad);
};

#endif // SCENE_H

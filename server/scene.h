#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QRandomGenerator>

#include "player.h"
#include "answer.h"
#include "food.h"
#include "question.h"

class scene
{
public:

    static int bits;
    static int operandsCount;
    static std::string operands;

    int generator = 5;
    std::string expr;

    scene();

    std::vector<Answer> answers;

    std::vector<Food> food;

    std::vector<Player> players;

    void new_food(int i);

    void new_answer(int i);

    void new_player(QString name);

    bool collision(Entity a, Entity b);

    void generate_answers();

    void generate_food();

    void update_player(int iter, QString name, double angle, int red_color = 0, int green_color = 255, int blue_color = 0);

    void update_answer(int id);

    void update_numbers(int correct);

    std::vector<Answer> get_answers();

    std::vector<Player> get_players();

    std::vector<Food> get_food();

    std::vector<int> updated_food;

    std::vector<int> get_updated_food();

    void check_correct(int i);

    void update(int clientID);
};

#endif // SCENE_H

#ifndef SCENE_H
#define SCENE_H

#include <QVector>
#include <QRandomGenerator>
#include <QMediaPlayer>
#include "player.h"
#include "answer.h"
#include "food.h"
#include "question.h"
#include "bot.h"


class scene
{
public:

    static int bits;
    static int operandsCount;
    static std::string operands;

    int generator = 5;
    std::string expr;

    scene();

private:
    const double one_point = sqrt(1 / 3.14);

    std::vector<Answer> answers;

    std::vector<Food> food;

    std::vector<Player> players;

    std::vector<Bot> bots;

    void new_food(int i);

    void new_answer(int i);

    void new_bot(int i);

    bool collision(Entity a, Entity b);

    bool collision_players(Player a, Player b);

    void generate_answers();

    void generate_food();

    void generate_bots();

    void check_correct(int i);

    void update_answer(int id);

    void update_bots();

    void update_numbers(int correct);

    void soundGood();

    void soundBad();

    std::vector<int> updated_food;

    int minimal_online_player = 1e6;

public:
    void update_player(int iter, QString name, double angle, int red_color = 0, int green_color = 255, int blue_color = 0);

    void new_player(QString name);

    std::vector<Answer>& get_answers();

    std::vector<Player>& get_players();

    std::vector<Food>& get_food();

    std::vector<Bot>& get_bots();

    std::vector<int>& get_updated_food();

    void clear_updated_food();

    void update(int clientID);

    void disconnected_player(int clientID);
};

#endif // SCENE_H

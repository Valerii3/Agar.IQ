#include "scene.h"

int scene::bits = 1;
int scene::operandsCount = 1;
std::string scene::operands = "+-*";

void scene::update_numbers(int correct) {
    answers[0].update_number(correct);
    for (int i = 1; i < 10; i++) {
        answers[i].update_number(correct + rand() % 10 - 5);
    }
}

scene::scene()
{
    generate_answers();
    generate_food();

    auto question = Question(bits, operandsCount, operands);

    expr = question.get_question();
    generator = question.get_answer();
    update_numbers(generator);
}

void scene::new_answer(int i) {
    answers[i] = Answer(i);
}

void scene::new_food(int i) {
    auto a = Food(i);
    for (auto player : players) {
        while (collision(player, a)) {
            a = Food(i);
        }
    }
    food[i] = a;
}

void scene::new_player(QString name) {
    players.push_back(Player(name, rand(), rand(), 30, players.size()));
}

void scene::generate_answers() {
    answers.resize(10);
    for (int i = 0; i < 10; i++) {
        new_answer(i);
        for (int k = 0; k < players.size(); k++) {
            for (int j = 0; j < i; j++) {
                if (collision(answers[i], answers[j]) || collision(players[k], answers[i])) {
                    new_answer(i);
                    i--;
                    j = 0;
                }
            }
        }
    }
}

void scene::generate_food() {
    for (int i = 0; i < 40; i++) {
        auto a = Food(i);
        for (auto player : players) {
            while (collision(player, a)) {
                a = Food(i);
            }
        }
        food.push_back(a);
    }
}

void scene::update_player(int iter, QString name, double x, double y, double rad) {
    players[iter].player_name = name.toStdString();
    players[iter].radius = rad;
    players[iter].x_coordinate = x;
    players[iter].y_coordinate = y;
}

bool scene::collision(Entity a, Entity b) {
    auto x1 = a.get_x_position();
    auto y1 = a.get_y_position();
    auto r1 = a.get_radius();

    auto x2 = b.get_x_position();
    auto y2 = b.get_y_position();
    auto r2 = b.get_radius();

    if (b.type == "player") {
        if (pow(x1 - x2, 2) + pow(y1 - y2, 2) <= pow(r2 - r1, 2)) {
            return true;
        }
        return false;
    } else {
        if (pow(x1 - x2, 2) + pow(y1 - y2, 2) <= pow(r2 + r1 + 5, 2) ){
            return true;
        }
        return false;
    }
}

std::vector<Player> scene::get_players() {
    return players;
}

std::vector<Answer> scene::get_answers() {
    return answers;
}

std::vector<Food> scene::get_food() {
    return food;
}

void scene::check_correct(int i) {
    if (answers[i].get_number() == generator) {
        new_answer(i);
        auto question = Question(bits, operandsCount, operands);

        expr = question.get_question();
        generator = question.get_answer();
        update_numbers(generator);
    } else {
        new_answer(i);
    }
}

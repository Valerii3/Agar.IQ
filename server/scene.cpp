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
    auto a = Answer();
    for (int k = 0; k < players.size(); k++) {
        for (int j = 0; j < i; j++) {
            if (collision(a, answers[j]) || collision(a, players[k])) {
                a = Answer();
                i--;
                j = 0;
            }
        }
    }
    answers[i] = a;
}

void scene::new_food(int i) {
    auto a = Food();
    for (auto player : players) {
        while (collision(a, player)) {
            a = Food();
        }
    }
    food[i] = a;
}

void scene::new_player(QString name) {
    players.push_back(Player(name, 250.0 + rand() % 1400, 100.0 + rand() % 700, 30, players.size()));
}

void scene::generate_answers() {
    answers.resize(10);
    for (int i = 0; i < 10; i++) {
        new_answer(i);
    }
}

void scene::generate_food() {
    food.resize(40);
    for (int i = 0; i < 40; i++) {
        new_food(i);
    }
}

void scene::update_player(int iter, QString name, double angle) {
    players[iter].player_name = name.toStdString();
    players[iter].player_angle = angle;
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

void scene::update(int clientID) {
    players[clientID].x_coordinate += players[clientID].player_speed * cos(players[clientID].player_angle);
    players[clientID].y_coordinate += players[clientID].player_speed * sin(players[clientID].player_angle);

    for (int i = 0; i < answers.size(); i++) {
        if (collision(answers[i], players[clientID])) {
            if (answers[i].get_number() == generator) {
                players[clientID].score += 3;
                players[clientID].is_correct = "Correct!";
                players[clientID].radius = std::min(players[clientID].get_radius() + sqrt(3 / 3.14), 60.0);
                generator = 1 + rand() % 19;

                new_answer(i);
                auto question = Question(bits, operandsCount, operands);

                expr = question.get_question();
                generator = question.get_answer();
                update_numbers(generator);
            } else {
                players[clientID].score -= 10;
                players[clientID].is_correct = "Wrong!";
                players[clientID].radius = std::max(players[clientID].get_radius() - sqrt(6 / 3.14), 7.0);
                generator = 1 + rand() % 19;

                new_answer(i);
                auto question = Question(bits, operandsCount, operands);

                expr = question.get_question();
                generator = question.get_answer();
                update_numbers(generator);
            }
        }
    }

    for (int i = 0; i < food.size(); i++) {
        if (collision(food[i], players[clientID])) {
            players[clientID].score += 1;
            players[clientID].is_correct = "";
            players[clientID].radius = std::min(players[clientID].get_radius() + sqrt(1 / 3.14), 60.0);

            new_food(i);
        }
    }
}

#include "scene.h"

int scene::bits = 1;
int scene::operandsCount = 1;
std::string scene::operands = "+-*";

void scene::update_numbers(int correct) {
    answers[0].update_number(correct);
    for (int i = 1; i < 20; i++) {
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
    answers.resize(20);
    for (int i = 0; i < 20; i++) {
        new_answer(i);
    }
}

void scene::generate_food() {
    food.resize(150);
    for (int i = 0; i < 150; i++) {
        new_food(i);
    }
}

void scene::update_player(int iter, QString name, double angle, int red_color, int green_color, int blue_color) {
    players[iter].player_name = name.toStdString();
    players[iter].player_angle = angle;

    players[iter].blue_color = blue_color;
    players[iter].green_color = green_color;
    players[iter].red_color = red_color;
}

bool scene::collision(Entity a, Entity b) {
    auto x1 = a.get_x_position();
    auto y1 = a.get_y_position();
    auto r1 = a.get_radius();

    auto x2 = b.get_x_position();
    auto y2 = b.get_y_position();
    auto r2 = b.get_radius();

    if (b.type == "player" || a.type == "player") {
        if (pow(x1 - x2, 2) + pow(y1 - y2, 2) <= pow(r2 + r1, 2)) {
            return true;
        }
        return false;
    } else {
        if (pow(x1 - x2, 2) + pow(y1 - y2, 2) <= pow(r2 + r1 + 15, 2) ){
            return true;
        }
        return false;
    }
}

bool scene::collision_players(Player a, Player b) {
    auto x1 = a.get_x_position();
    auto y1 = a.get_y_position();
    auto r1 = a.get_radius();

    auto x2 = b.get_x_position();
    auto y2 = b.get_y_position();
    auto r2 = b.get_radius();

    if (!a.is_online && !b.is_online) {
        return false;
    }
    if (pow(x1 - x2, 2) + pow(y1 - y2, 2) <= pow(r2 - r1, 2)) {
        return true;
    }
    return false;
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

    players[clientID].x_coordinate = std::min(players[clientID].x_coordinate, 2505.0 - players[clientID].get_radius());
    players[clientID].x_coordinate = std::max(players[clientID].x_coordinate, -505.0 + players[clientID].get_radius());

    players[clientID].y_coordinate += players[clientID].player_speed * sin(players[clientID].player_angle);

    players[clientID].y_coordinate = std::min(players[clientID].y_coordinate, 1205.0 - players[clientID].get_radius());
    players[clientID].y_coordinate = std::max(players[clientID].y_coordinate, -305.0 + players[clientID].get_radius());

    for (int i = 0; i < answers.size(); i++) {
        if (collision(answers[i], players[clientID])) {
            if (answers[i].get_number() == generator) {
                players[clientID].score += 3;
                players[clientID].is_correct = "Correct!";
                players[clientID].radius = std::min(players[clientID].get_radius() + sqrt(3 / 3.14), 90.0);
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

            players[clientID].player_speed = (9.5 / players[clientID].get_radius()) + 3.5;
        }
    }

    for (int i = 0; i < food.size(); i++) {
        if (collision(food[i], players[clientID])) {
            players[clientID].score += 1;
            players[clientID].is_correct = "";
            players[clientID].radius = std::min(players[clientID].get_radius() + sqrt(1 / 3.14), 90.0);

            new_food(i);

            updated_food.push_back(i);

            players[clientID].player_speed = (9.5 / players[clientID].get_radius()) + 3.5;
        }
    }

    for (int i = 0; i < players.size(); i++) {
        if (i == clientID) {
            continue;
        }

        if (players[i].is_eaten || !players[i].is_online) {
            continue;
        }

        if (collision_players(players[i], players[clientID])) {
            if (players[clientID].score > players[i].score) {
                players[clientID].score += players[i].score;
                players[clientID].radius = std::min(players[clientID].get_radius() + sqrt(players[i].score / 3.14), 90.0);

                players[clientID].player_speed = (9.5 / players[clientID].get_radius()) + 3.5;
                players[clientID].is_correct = "";

                players[i].eaten();
            }

            if (players[clientID].score < players[i].score) {
                players[i].score += players[clientID].score;
                players[i].radius = std::min(players[i].get_radius() + sqrt(players[clientID].score / 3.14), 90.0);

                players[i].player_speed = (9.5 / players[i].get_radius()) + 3.5;
                players[i].is_correct = "";

                players[clientID].eaten();
            }
        }
    }
}

std::vector<int> scene::get_updated_food() {
    return updated_food;
}

void scene::disconnected_player(int clientID) {
    players[clientID].is_online = 0;
    players[clientID].is_eaten = 1;
}

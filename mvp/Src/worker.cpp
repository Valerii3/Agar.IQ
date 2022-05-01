#include "../Include/worker.h"

using namespace std::chrono_literals;

int Worker::bits = 1;
int Worker::operandsCount = 1;
std::string Worker::operands = "+-*";

void Worker::generate_answers(int correct) {
    if (!answers_data.empty()) {
        answers_data[0].update_number(correct);
        for (int i = 1; i < 10; i++) {
            answers_data[i].update_number(correct);
        }
    }
}

Worker::Worker(QObject *parent)
    : QObject{parent}
{
    srand(time(0));

    Player player;

    auto question = Question(bits, operandsCount, operands);

    eaten_answers = {};
    eaten_foods = {};

    expr = question.getQuestion();
    generator = question.getAnswer();
    generate_answers(generator);
}

void Worker::doWork() {
    std::chrono::nanoseconds timeStep(16ms);
    using clock = std::chrono::high_resolution_clock;
    std::chrono::nanoseconds lag(0ns);
    auto timeStart = clock::now();

    while (!quitGame) {
        auto deltaTime = clock::now() - timeStart;
        timeStart = clock::now();
        lag += std::chrono::duration_cast<std::chrono::nanoseconds>(deltaTime);
        while (lag >= timeStep){
            lag -= timeStep;
            update();
            emit signalResultReady();
        }
    }
}

bool Worker::collision(Entity a, Entity b) {
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

void Worker::update() {
    player.x_position -= player.speed * cos(player.angle);
    player.y_position -= player.speed * sin(player.angle);

    srand(time(0));
    for (int i = 0; i < answers_data.size(); i++) {
        if (collision(answers_data[i], player)) {
            eaten_answers.push_back(i);
            if (answers_data[i].get_number() == generator) {
                score += 3;
                is_correct = "Correct!";
                player.radius = std::min(player.get_radius() + sqrt(3 / 3.14), 60.0);
                generator = 1 + rand() % 19;

                auto question = Question(bits, operandsCount, operands);

                expr = question.getQuestion();
                generator = question.getAnswer();
                generate_answers(generator);
            } else {
                score -= 10;
                is_correct = "Wrong!";
                player.radius = std::max(player.get_radius() - sqrt(6 / 3.14), 7.0);
                generator = 1 + rand() % 19;

                auto question = Question(bits, operandsCount, operands);

                expr = question.getQuestion();
                generator = question.getAnswer();
                generate_answers(generator);
            }
        }
    }

    for (int i = 0; i < foods_data.size(); i++){
        if (collision(foods_data[i], player)) {
            eaten_foods.push_back(i);
            score += 1;
            is_correct = "";
            player.radius = std::min(player.get_radius() + sqrt(1 / 3.14), 60.0);
        }
    }
}

void Worker::slotQuitGame(bool value) {
    quitGame = value;
}

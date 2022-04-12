#include "worker.h"

using namespace std::chrono_literals;

int Worker::bits = 1;
int Worker::operandsCount = 1;
std::string Worker::operands = "+-*";

void Worker::generate_answers(int correct) {
    answers.clear();
    answers.push_back(Answer{correct});
    for (int i = 1; i < 10; i++) {
        answers.push_back(Answer{correct - 5 + rand() % 10});
        for (int j = 0; j < i; j++) {
            if (collision(answers[i], answers[j]) || collision(player, answers[i])) {
                answers[i] = Answer{correct - 5 + rand() % 10};
                i--;
                j = 0;
            }
        }
    }
}

void Worker::generate_food() {
    auto a = Food();
    while (collision(player, a)) {
        a = Food();
    }
    food.push_back(a);
}

Worker::Worker(QObject *parent)
    : QObject{parent}
{
    srand(time(0));

    Player player;

    auto question = Question(bits, operandsCount, operands);

    expr = question.getQuestion();
    generator = question.getAnswer();
    generate_answers(generator);

    for (int i = 0; i < 80; i++) {
        food.push_back(Food());
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

void Worker::doWork() {
    std::chrono::nanoseconds timeStep(16ms);
    using clock = std::chrono::high_resolution_clock;
    std::chrono::nanoseconds lag(0ns);
    auto timeStart = clock::now();

    while (!quitGame){
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

void Worker::update() {
    player.x_position -= player.speed * cos(player.angle);
    player.y_position -= player.speed * sin(player.angle);

    srand(time(0));
    for (int i = 0; i < answers.size(); i++) {
        if (collision(answers[i], player)) {
            if (answers[i].get_number() == generator) {
                score += 3;
                is_correct = "Correct!";
                player.radius = std::min(player.get_radius() + sqrt(3 / 3.14), 60.0);
                generator = 1 + rand() % 19;

                auto question = Question(bits, operandsCount, operands);

                expr = question.getQuestion();
                generator = question.getAnswer();
                generate_answers(generator);
            } else {
                //quitGame = true;
                //emit signalGameFinish();
                score -= 3;
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

    for (int i = 0; i < food.size(); i++){
        if (collision(food[i], player)) {
            food.erase(food.begin() + i);
            generate_food();
            score += 1;
            is_correct = "";
            player.radius = std::min(player.get_radius() + sqrt(1 / 3.14), 60.0);
        }
    }
}

void Worker::slotQuitGame(bool value) {
    quitGame = value;
}

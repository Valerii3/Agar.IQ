#include "worker.h"

using namespace std::chrono_literals;

int Worker::bits = 1;
int Worker::operandsCount = 1;
std::string Worker::operands = "+-*";

Worker::Worker(QObject *parent) : QObject{parent} {
    srand(time(0));

    Player player;

    for (int i = 1; i < 22; i++) {
        questions.push_back(Question(bits, operandsCount, operands));
        food.push_back(Food());
        food.push_back(Food());

        bool flag = true;
        while (true) {
            flag = true;
            for (unsigned long long i = 0; i < questions.size() - 1; i++) {
                for (unsigned long long j = i + 1; j < questions.size(); j++) {
                    Answer *a = questions[i].getAnswer(), *b = questions[j].getAnswer();
                    if (collision(a->get_x_position(), a->get_y_position(), a->get_radius(),
                                  b->get_x_position(), b->get_y_position(), b->get_radius())) {
                        a->x_position += 15;
                        a->y_position += 15;
                        flag = false;
                    }
                }
            }
            if (flag) {
                break;
            }
        }
    }
}

bool Worker::collision(double x1, double y1, double r1, double x2, double y2, double r2) {
    if (pow(x1 - x2, 2) + pow(y1 - y2, 2) <= pow(r1 + r2, 2)) {
        return true;
    }
    return false;
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
        while (lag >= timeStep) {
            lag -= timeStep;
            update();
            emit signalResultReady();
        }
    }
}

void Worker::update() {
    player.x_position += player.speed_X;
    player.y_position += player.speed_Y;

    int ans = questions[currQuestion].getAnswer()->get_number();
    for (unsigned long long i = 0; i < questions.size(); i++) {
        Answer *a = questions[i].getAnswer();
        if (collision(a->get_x_position(), a->get_y_position(), a->get_radius(),
                      player.get_x_position(), player.get_y_position(), player.get_radius())) {
            if (a->get_number() == ans) {
                questions.erase(questions.begin() + i);
                score += 10;
                player.radius += 1;
                currQuestion = rand() % questions.size();
                break;
            } else {
                quitGame = true;
                emit signalGameFinish();
            }
        }
    }

    for (unsigned long long i = 0; i < food.size(); i++) {
        if (collision(food[i].get_x_position(), food[i].get_y_position(), food[i].get_radius(),
                      player.get_x_position(), player.get_y_position(), player.get_radius())) {
            food.erase(food.begin() + i);
            score += 3;
            player.radius += 0.4;
        }
    }
}

void Worker::slotQuitGame(bool value) {
    quitGame = value;
}

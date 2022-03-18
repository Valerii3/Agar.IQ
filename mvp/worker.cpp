#include "worker.h"

using namespace std::chrono_literals;

Worker::Worker(QObject *parent)
        : QObject{parent}
{
    srand(time(0));

    Player player;

    for (int i = 1; i < 40; i++) {
        answers.push_back(Answer{i / 2});
        food.push_back(Food());
        food.push_back(Food());

        bool flag = true;
        while (true) {
            flag = true;
            for (int i = 0; i < answers.size() - 1; i++){
                for (int j = i + 1; j < answers.size(); j++) {
                    if (collision(answers[i].get_x_position(), answers[i].get_y_position(), answers[i].get_radius(),
                                  answers[j].get_x_position(), answers[j].get_y_position(), answers[j].get_radius())){
                        answers[i].x_position += 15;
                        answers[i].y_position += 15;
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

bool Worker::collision(double x1, double y1, double r1, double x2, double y2, double r2){
    if (pow(x1-x2, 2) + pow(y1-y2, 2) <= pow(r1 + r2, 2) ){
        return true;
    }
    return false;
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
    player.x_position += player.speed_X;
    player.y_position += player.speed_Y;

    srand(time(0));
    for (int i = 0; i < answers.size() - 1; i++){
        if (collision(answers[i].get_x_position(), answers[i].get_y_position(), answers[i].get_radius(),
                      player.get_x_position(), player.get_y_position(), player.get_radius())){

            if (answers[i].get_number() == generator){
                answers.erase(answers.begin() + i);
                score += 10;
                player.radius += 1;
                generator = 1 + rand() % 19;
                expr = rndExpr(generator);
            } else {
                quitGame = true;
                emit signalGameFinish();
            }
        }
    }

    for (int i = 0; i < food.size() - 1; i++){
        if (collision(food[i].get_x_position(), food[i].get_y_position(), food[i].get_radius(),
                      player.get_x_position(), player.get_y_position(), player.get_radius())){

            food.erase(food.begin() + i);
            score += 3;
            player.radius += 0.4;
        }
    }
}

void Worker::slotQuitGame(bool value) {
    quitGame = value;
}

std::string Worker::rndExpr(int &generator) {
    srand(time(0));
    int a = rand() % 10;
    int b = rand() % 10;
    generator = a + b;
    std::string expr = std::to_string(a) + " + " + std::to_string(b) + " = ?";
    return expr;
}
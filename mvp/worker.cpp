#include "worker.h"

using namespace std::chrono_literals;

Worker::Worker(QObject *parent)
    : QObject{parent}
{
    srand(time(0));
    entities.push_back(Entity{});
    for (int i = 1; i < 20; i++){
        entities.push_back(Entity{
                               250.0 + rand() % 1400,
                               100.0 + rand() % 700,
                               50,
                               0,
                               0,
                               i,
                               Qt::red
                           });
    }

    bool flag = true;
    while (true){
        flag = true;
        for (int i = 0; i < entities.size() - 1; i++){
            for (int j = i+1; j < entities.size(); j++){
                if (collision(entities[i].x, entities[i].y, entities[i].r, entities[j].x, entities[j].y, entities[j].r)){
                    entities[i].x += 5;
                    entities[i].y += 5;
                    flag = false;
                }
            }
        }
        if (flag){
            break;
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

void Worker::update(){
    for (auto &it : entities){
        it.x += it.speedX;
        it.y += it.speedY;
    }

    srand(time(0));
    for (int i = 0; i < entities.size() - 1; i++){
        for (int j = i+1; j < entities.size(); j++){
            if (collision(entities[i].x, entities[i].y, entities[i].r, entities[j].x, entities[j].y, entities[j].r)){

                if (entities[j].num == generator){
                    entities.erase(entities.begin() + j);
                    score += 5;
                    generator = 1 + rand() % 19;
                    expr = rndExpr(generator);
                } else {
                    quitGame = true;
                    emit signalGameFinish();
                }
            }
        }
    }
}

void Worker::slotQuitGame(bool value){
    quitGame = value;
}

std::string Worker::rndExpr(int &generator){
    srand(time(0));
    int a = rand() % 10;
    int b = rand() % 10;
    generator = a + b;
    std::string expr = std::to_string(a) + " + " + std::to_string(b) + " = ?";
    return expr;
}

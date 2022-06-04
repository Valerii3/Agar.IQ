#include "../Include/worker.h"

using namespace std::chrono_literals;

int Worker::bits = 1;
int Worker::operandsCount = 1;
std::string Worker::operands = "+-*";

Worker::Worker(QObject *parent)
    : QObject{parent} {
    srand(time(0));
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
            emit signalResultReady();
        }
    }
}

void Worker::slotQuitGame(bool value) {
    quitGame = value;
}

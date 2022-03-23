#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include <chrono>
#include <vector>
#include <ctime>

#include "question.h"
#include "food.h"
#include "player.h"

class Worker : public QObject {
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

    static int bits;
    static int operandsCount;
    static std::string operands;
    std::vector <Question> questions;
    std::vector <Food> food;
    int currQuestion = 0;
    QString text = "Score: ";
    int score = 0;
    Player player;

public
    slots:
            void doWork();

    void slotQuitGame(bool value);

    signals:
            void signalResultReady();

    void signalGameFinish();

private:
    bool quitGame = false;

    void update();

    bool collision(double x1, double y1, double r1, double x2, double y2, double r2);
};

#endif // WORKER_H

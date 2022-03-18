#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QDebug>
#include <chrono>
#include <vector>
#include <ctime>

#include "entity.h"
#include "answers.h"
#include "food.h"
#include "player.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    std::vector<Answer> answers;
    std::vector<Food> food;
    int generator = 5;
    QString text = "Score: ";
    int score = 0;
    std::string rndExpr(int &generator);
    std::string expr = rndExpr(generator);

    Player player;

public slots:
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
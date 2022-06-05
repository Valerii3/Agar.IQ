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
#include "question.h"
#include "bot.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    static int bits;
    static int operandsCount;
    static std::string operands;

    explicit Worker(QObject *parent = nullptr);

    QString is_correct = "";

    QString text = "Score: ";
    int score = 0;

    int generator = 5;

    std::string expr;

    QVector<Player> players_data;
    QVector<Answer> answers_data;
    QVector<Food> food_data;
    QVector<Bot> bots_data;

    Player player;

public slots:
    void doWork();
    void slotQuitGame(bool value);

signals:
    void signalResultReady();
    void signalGameFinish();

private:
    bool quitGame = false;
};



#endif // WORKER_H

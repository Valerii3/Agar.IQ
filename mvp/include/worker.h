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
#include "generator.h"
#include "question.h"

class Worker : public QObject
{
    Q_OBJECT
public:
    static int bits;
    static int operandsCount;
    static std::string operands;

    explicit Worker(QObject *parent = nullptr);
    std::vector<Answer> answers;
    std::vector<Food> food;

    QString is_correct = "";

    QString text = "Score: ";
    int score = 0;

    int generator = 5;

    std::string expr;

    void generate_answers(int correct);
    void generate_food();

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
    bool collision(Entity a, Entity b);
};



#endif // WORKER_H

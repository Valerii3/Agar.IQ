#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QColor>
#include <QDebug>
#include <chrono>
#include <vector>
#include <ctime>

struct Entity {
    double x = 40, y = 500, r = 30;
    double speedX = 0, speedY = 0;
    int num;
    QColor color = Qt::green;
};


class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);
    std::vector<Entity> entities;
    int generator = 5;
    QString text = "Score: ";
    int score = 0;
    std::string rndExpr(int &generator);
    std::string expr = rndExpr(generator);

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

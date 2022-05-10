#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QThread>
#include <QPainter>
#include <QTcpSocket>
#include <QPaintEvent>
#include "worker.h"
#include <json.hpp>

using json = nlohmann::json;

namespace Ui {
class Scene;
}

class Scene : public QWidget
{
    Q_OBJECT

public:
    explicit Scene(QWidget *parent = nullptr);
    ~Scene();

private:

    bool in_bounds(Entity dot);

    QTcpSocket* socket;
    QString name = "Alex";
    int clientID = 0;

    bool initialization = true;
    QVector<Food> initialization_food;
    QVector<Answer> initialization_answers;

public slots:
    void slotResultReady();
    void slotGameFinish();
    void startGame();

private slots:
    void readFromServer();
    void sendToServer();

signals:
    void signalQuitGame(bool value);
    void startWork();
    void first();

private:
    Ui::Scene *ui;
    QThread workerThread;
    Worker *worker = nullptr;
    bool isMenu = true;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void on_pushButton_clicked();
};

#endif // SCENE_H

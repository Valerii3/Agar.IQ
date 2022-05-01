#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QThread>
#include <QPainter>
#include <QTcpSocket>
#include <QPaintEvent>
#include "worker.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

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
    QTcpSocket* socket;
    QByteArray Data;
    QString name = "Alex";
    int server_iterator = 0;

    QVector<Player> players_data;

    QJsonDocument doc;
    QJsonParseError docError;

public slots:
    void slotResultReady();
    void slotGameFinish();
    void startGame();

private slots:
    void slotReadyRead();
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
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private slots:
    void on_pushButton_clicked();
};

#endif // SCENE_H

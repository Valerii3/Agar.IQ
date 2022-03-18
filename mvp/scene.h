#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include <QThread>
#include <QPainter>
#include <QPaintEvent>
#include "worker.h"

namespace Ui {
    class Scene;
}

class Scene : public QWidget
{
    Q_OBJECT

public:
    explicit Scene(QWidget *parent = nullptr);
    ~Scene();

public slots:
            void slotResultReady();
    void slotGameFinish();

    signals:
            void signalQuitGame(bool value);
    void startWork();

private:
    Ui::Scene *ui;
    QThread workerThread;
    Worker *worker = nullptr;
    bool isMenu = true;

protected:
    void paintEvent(QPaintEvent* event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private slots:
            void on_startGameButton_clicked();
};

#endif // SCENE_H
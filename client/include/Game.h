#ifndef CLIENT_GAME_H
#define CLIENT_GAME_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QtCore>

#include "Map.h"

const int DIFFERENCE_SCENE_VIEW = 50;

struct Game : public QMainWindow {
private:

Q_OBJECT

public:
    QGraphicsView * graphicsView;

    static Game* getInstance();
    ~Game();

private:
    explicit Game(QWidget *parent = 0);
    static Game* gamePtr;
    Map * myMap;
};

#endif //CLIENT_GAME_H

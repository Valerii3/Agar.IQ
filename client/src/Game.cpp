#include "include/Game.h"

Game* Game::gamePtr = nullptr;

Game::Game( QWidget *parent ) : QMainWindow( parent ) {
    graphicsView = new QGraphicsView();
    myMap = Map::get_instance();
    graphicsView->setScene(myMap);
}

Game::~Game() {
    delete gamePtr;
}

Game *Game::getInstance() {
    if ( !gamePtr ) {
        gamePtr = new Game();
    }
    return gamePtr;
}


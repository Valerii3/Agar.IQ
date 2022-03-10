#include "include/ElementsFactory.h"

ElementsFactory* ElementsFactory::get_instance() {
    if (!elements_factory_ptr) {
        elements_factory_ptr = new ElementsFactory();
    }

    return elements_factory_ptr;
}

ElementsFactory::~ElementsFactory() {
    delete elements_factory_ptr;
}

Food *ElementsFactory::createFood(QString x, QString y, QString c) {
    return new Food(x, y, c);
}

Bot *ElementsFactory::createBot( QString x , QString y , QString c , QString r , QString s , QString n ) {
    return new Bot( x , y, c, r, s, n );
}

Player *ElementsFactory::createPlayer(QString x , QString y , QString c , QString r , QString s , QString n) {
    return new Player(x , y, c, r, s, n);
}

EjectedFood *ElementsFactory::createEjectedFood(QString xP, QString yP,QString cP) {
    return new EjectedFood(xP, yP, cP);
}

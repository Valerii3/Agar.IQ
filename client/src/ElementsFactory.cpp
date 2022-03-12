#include "include/ElementsFactory.h"

ElementsFactory* ElementsFactory::elements_factory_ptr = nullptr;

ElementsFactory* ElementsFactory::get_instance() {
    if (!elements_factory_ptr) {
        elements_factory_ptr = new ElementsFactory();
    }

    return elements_factory_ptr;
}

ElementsFactory::~ElementsFactory() {
    delete elements_factory_ptr;
}

Food *ElementsFactory::create_food(QString x, QString y, QString c) {
    return new Food(x, y, c);
}

Player *ElementsFactory::create_player(QString x , QString y , QString c , QString r , QString s , QString n) {
    return new Player(x , y, c, r, s, n);
}

EjectedFood *ElementsFactory::create_ejected_food(QString xP, QString yP,QString cP) {
    return new EjectedFood(xP, yP, cP);
}

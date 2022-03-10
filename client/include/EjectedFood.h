#ifndef CLIENT_EJECTED_FOOD_H
#define CLIENT_EJECTED_FOOD_H

#include <QList>
#include "Element.h"

struct EjectedFood : public Element {

public:
    EjectedFood();
    virtual void is_eaten();

    void paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)

    virtual QString who();
};


#endif //CLIENT_EJECTED_FOOD_H

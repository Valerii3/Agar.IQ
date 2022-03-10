#ifndef CLIENT_FOOD_H
#define CLIENT_FOOD_H

#include "Element.h"

struct Food : public Element {
private:
    const int FOOD_RADIUS = 6;

public:
    Food(QString xP, QString yP, QString cP);

    virtual void is_eaten();
    virtual QString who();

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
};

#endif //CLIENT_FOOD_H

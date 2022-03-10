#ifndef CLIENT_MOVINGELEMENT_H
#define CLIENT_MOVINGELEMENT_H

#include <QList>

#include "Element.h"

struct MovingElement : public Element {

public:
    MovingElement();
    virtual void eat_element(const Element* to_be_eaten) = 0;
    int score_;

    void check_collision();
};

#endif //CLIENT_MOVINGELEMENT_H

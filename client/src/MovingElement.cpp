#include "include/MovingElement.h"

MovingElement::MovingElement() {
    score_ = 0;
}

void MovingElement::check_collision() {
    QList<QGraphicsItem*> collisions = QGraphicsItem::collidingItems();

    foreach (QGraphicsItem* item, collisions) {
        if(Element* element = dynamic_cast<Element*>(item)) {
            if(element->get_radius() < radius_) {
                eat_element(element);
            }
        }

    }
}
// end checkCollision
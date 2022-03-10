#include "include/MovingElement.h"

MovingCell::MovingCell() {
    score_ = 0;
}

void MovingCell::check_collision() {
    QList<QGraphicsItem*> collisions = QGraphicsItem::collidingItems();

    foreach (QGraphicsItem* item, collisions) {
        if(Elementl* element = dynamic_cast<Element*>(item)) {
            if(element->get_radius() < radius_) {
                eat_element(element);
            }
        }

    }
}
// end checkCollision

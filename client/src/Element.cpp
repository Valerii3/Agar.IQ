#include "include/Element.h"


Element::Element(QGraphicsEllipseItem *parent) : QGraphicsEllipseItem(parent) {
    has_been_eaten_ = false;
}

qreal Element::get_x_position() const {
    return x_position_;
}

qreal Element::get_y_position() const {
    return y_position_;
}

int Element::get_radius() const {
    return radius_;
}

qreal Element::get_top_edge() const {
    return y_position_ - radius_;
}

qreal Element::get_left_edge() const {
    return x_position_ - radius_;
}

QRectF Element::bounding_rect() const {
    return QRectF( x_position_ - radius_, y_position_ - radius_,
                   QRECT_SCALE * radius_, QRECT_SCALE * radius_ );
}

QPainterPath Element::shape() const {
    QPainterPath path;
    path.addEllipse(bounding_rect());
    return path;
}
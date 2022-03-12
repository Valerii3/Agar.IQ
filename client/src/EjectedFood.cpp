#include "include/EjectedFood.h"
#include "include/GameWindow.h"

EjectedFood::EjectedFood( QString xD, QString yD, QString cP) {
    radius_ = EJECTED_FOOD_RADIUS;
    color_.setNamedColor( QColor::colorNames().at( cP.toInt() ) );
    x_position_ = xD.toDouble();
    y_position_ = yD.toDouble();

    setRect(get_left_edge(), get_top_edge(), QRECT_SCALE * radius_, QRECT_SCALE * radius_);
    setPos(get_left_edge(), get_top_edge());
}

void EjectedFood::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {

    // reference to GameWindow

    painter->setBrush(color_);
    painter->drawEllipse( boundingRect() );
}

void EjectedFood::is_eaten() {
    QString foodEaten = "ef|";
    foodEaten += QString::number(this->get_x_position());
    foodEaten += "|";
    foodEaten += QString::number(this->get_y_position());
    foodEaten += "|";

    LoginWindow::getInstance()->sendInfo( foodEaten );
}

QString EjectedFood::who() {
    QString ej = "EjectedFood";
    return ej;
}

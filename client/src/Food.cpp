#include "include/GameWindow.h"
#include "include/Food.h"

Food::Food(QString xP, QString yP, QString cP) {
    radius_ = FOOD_RADIUS;

    x_position_ = xP.toDouble();
    y_position_ = yP.toDouble();

    color_.setNamedColor( QColor::colorNames().at( cP.toInt() ) );

    setRect(get_left_edge(), get_top_edge(), QRECT_SCALE * radius_, QRECT_SCALE * radius_);
    setPos(get_left_edge(), get_top_edge());
}

void Food::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {

    // reference to GameWindow

    painter->setBrush(color_);
    painter->drawEllipse( boundingRect() );
}

void Food::is_eaten() {
    QString food_eaten = "f|";
    food_eaten += QString::number(this->get_x_position());
    food_eaten += "|";
    food_eaten += QString::number(this->get_y_position());
    food_eaten += "|";

    LoginWindow::getInstance()->sendInfo( food_eaten );
}

QString Food::who() {
    return static_cast<QString>("Food");
}

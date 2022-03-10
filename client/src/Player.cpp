#include "include/GameWindow.h"
#include "include/Player.h"

Player::Player( QString x, QString y, QString c, QString r, QString s, QString n ) {
    x_position_ = x.toDouble();
    y_position_ = y.toDouble();
    radius_ = r.toInt();

    color_.setNamedColor(QColor::colorNames().at(c.toInt()));
    col = c.toInt();

    score_ = s.toInt();
    name_ = n;

    // reference to GameWindow

    setRect(x_position_ - radius_, y_position_ - radius_,
            QRECT_SCALE * radius_, QRECT_SCALE * radius_);
    setPos(x_position_ - radius_, y_position_ - radius_);

    setFlag(QGraphicsEllipseItem::ItemIsFocusable);
    setFocus();
}

float Player::speed() const {
    float speed = ( (PLAYER_SPEED_FACTOR) * radius_ ) + PLAYER_ADDITIONAL_SPEED_FACTOR;
    if(speed < PLAYER_MINIMUM_SPEED) {
        speed = PLAYER_MINIMUM_SPEED;
    }
    return speed;
}

void Player::mouse_move(qreal x, qreal y) {
    QPointF tmpVec = QPointF(x - xPosition_ , y - yPosition_);;

    qreal xDis = x_position_ - x;
    qreal yDis = y_position_ - y;

    double distance = sqrt(xDis * xDis + yDis * yDis); //sqrt(dx^2 + dy^2)

    if (distance != 0) {
        tmpVec = tmpVec / distance; // normalize vector
        targetVector_ = tmpVec;
    }

    if ( targetVector_.x() != 0 || targetVector_.y() != 0 ) {
        targetVector_ = targetVector_ / (sqrt( targetVector_.x() * targetVector_.x() + targetVector_.y() * targetVector_.y() ) ); // normalize

        qreal dx = (targetVector_.x() * speed()); // how far move in x direction
        qreal dy = (targetVector_.y() * speed()); // how far move in y direction

        x_position_ += dx;
        y_position_ += dy;
    }

    QString playerMove = "p|m|";
    playerMove += get_name();
    playerMove += "|";
    playerMove += QString::number( x_position_ );
    playerMove += "|";
    playerMove += QString::number( y_position_ );
    playerMove += "|";

    LoginWindow::getInstance()->sendInfo(playerMove);

    checkCollision();
}

void Player::eat_element(const Element* toBeEaten) {
    Cell* cell = const_cast<Element*>(to_be_eaten);
    QString what = cell->who();

    cell->is_eaten(); // change that cell status to eaten

    QString eatCell = "p|eatElement|";
    eatCell += getName();
    eatCell += "|";
    eatCell += QString::number(cell->get_radius());
    eatCell += "|";

    LoginWindow::getInstance() ->sendInfo(eat_element);
}

QString Player::who() {
    QString P = "Player";
    return P;
}

void Player::is_eaten() {
    QString playerLost = "p|lost|";
    playerLost += get_name();
    playerLost += "|";

    LoginWindow::getInstance()->sendInfo(playerLost);
}

void Player::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->setBrush(color_); // set color
    painter->drawEllipse( Cell::boundingRect() ); // draw rectangle

    painter->setBrush(Qt::black); // set text Brush
    painter->drawText(xPosition_ - DISPLACEMENT_X, yPosition_ + DISPLACEMENT_Y, name_); // set name on item
}

QString Player::get_name() {
    return name_;
}

void Player::setx(QString nx) {
    x_position_ = nx.toDouble();
}

void Player::sety(QString ny) {
    y_position_ = ny.toDouble();
}

void Player::advance(int step) {
    if (!step) {
        return;
    }

    for (int k = 0; k < Map::getInstance()->playerList.size(); k++) {
        if( Map::getInstance()->playerList[k]->getName() == Map::getInstance()->playerName ) {
            checkCollision();
            QString p = "p";
            LoginWindow::getInstance()->updateMonitor( p );

            break;
        }
    }
}

void Player::eject_food() {
    if( radius_ < 25 ) {
        return;
    }

    QPointF mousePos = QCursor::pos();

    double xD = mousePos.x() ;
    double yD = mousePos.y() ;

    qreal xDis = get_x_position() - xD;
    qreal yDis = get_y_position() - yD;

    if( xDis > 300 ) {
        xD /= 10;
    }
    if( yDis > 300 ) {
        yD /= 10;
    }

    QString ejectedFood = "new|ef|";
    ejectedFood += QString::number( xD );
    ejectedFood += "|";
    ejectedFood += QString::number( yD );
    ejectedFood += "|";
    ejectedFood += QString::number(col);
    ejectedFood += "|";
    ejectedFood += getName();
    ejectedFood += "|";

    LoginWindow::getInstance()->sendInfo(ejectedFood);
}

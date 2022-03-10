#ifndef CLIENT_PLAYER_H
#define CLIENT_PLAYER_H

#include <QGraphicsItem>
#include <QString>

#include "MovingElement.h"

const int DISPLACEMENT_X = 11;
const int DISPLACEMENT_Y = 4;

const float PLAYER_SPEED_FACTOR = (-1.0)/(32.0);
const float PLAYER_MINIMUM_SPEED = 0.5;
const float PLAYER_ADDITIONAL_SPEED_FACTOR = 2.5;

struct Player : public MovingElement {
public:

    Player( QString x, QString y, QString c, QString r, QString s, QString n );

    void mouse_move(qreal x, qreal y);
    virtual void eat_element(const Element* to_be_eaten);
    float speed() const;
    void eject_food();

    QString get_name();
    void setx( QString );
    void sety( QString );

    int col;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual void is_eaten();
    virtual QString who();

    void advance(int step) override;

private:
    QString name_;
    QPointF targetVector_;
};

#endif //CLIENT_PLAYER_H

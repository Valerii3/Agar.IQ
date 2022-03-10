#ifndef CLIENT_CELL_H
#define CLIENT_CELL_H

#include <QGraphicsEllipseItem>
#include <QPainter>
#include <QTime>
#include <QObject>

const int BOARD_WIDTH = 800;
const int BOARD_HEIGHT = 600;
const int QRECT_SCALE = 2;

struct Element : public QObject, public QGraphicsEllipseItem {

private:

Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:

    Element(QGraphicsEllipseItem *parent = NULL);

    qreal get_x_position() const;
    qreal get_y_position() const;
    int get_radius() const;

    qreal get_top_edge() const;
    qreal get_left_edge() const;

    virtual void is_eaten() = 0;
    virtual QString who() = 0;

    int radius_;
    bool has_been_eaten_;

protected:
    QPainterPath shape() const override ;
    QRectF bounding_rect() const override ;

    qreal x_position_, y_position_;
    QColor color_;
};


#endif //CLIENT_CELL_H

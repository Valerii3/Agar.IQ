#ifndef CLIENT_MAP_H
#define CLIENT_MAP_H

#include <QGraphicsScene>
#include <QCursor>
#include <QKeyEvent>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QTimer>
#include <QMessageBox>

#include "ElementsFactory.h"

const int VIEW_SCALE = 2;
const int FRAME_MS = 120;

struct Map : public QGraphicsScene {

public:

    static Map* get_instance();
    ~Map() override ;

    void food_element_is_eaten( QString, QString, QString, QString, QString );
    void get_new_food( QString, QString, QString);

    void get_new_player(QString, QString, QString, QString, QString, QString);
    void move_player( QString, QString, QString);
    void update_player( QString, QString, QString );
    void player_lost( QString );
    void player_eject_food(QString);
    void player_punish( QString );
    Player* this_player();

    void get_new_ejected_food(QString, QString, QString);
    void remove_ejected_food(QString, QString);

    virtual void advance();

    QString player_name;

    QList<Food*> food_list;
    QList<Player*> player_list;
    QList<EjectedFood*> ejected_food_list;

protected:
    void key_press_event(QKeyEvent *event) override ;
    void mouse_move_event( QGraphicsSceneMouseEvent *event ) override ;

private:
    Map();

    static Map* map_ptr = nullptr;
    QTimer* timer;
};

#endif //CLIENT_MAP_H

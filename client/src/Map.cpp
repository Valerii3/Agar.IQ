#include "include/LoginWindow.h"
#include "include/Map.h"

// reference to GameWindow

Map::Map() {
    timer = new QTimer();
    connect( timer, SIGNAL( timeout() ), this, SLOT( advance() ) );
    timer -> start( FRAME_MS );
    setBackgroundBrush( Qt::Dense7Pattern );
}

Map::~Map() {
    delete timer;
    delete map_ptr;
}

Map* Map::get_instance() {
    if (!map_ptr)
        mapPtr = new Map();
    return map_ptr;
}

void Map::advance() {
    for( int k = 0; k < playerList.size(); k++)
        playerList[k]->checkCollision();

    getInstance()->update();
}

void Map::key_press_event(QKeyEvent *event) {
    for( k = 0; k < playerList.size(); k++) {
        if ( playerList[k]->getName() == playerName ) {
            switch(event->key()) {
                case Qt::Key_F:
                    playerList[k]->ejectFood();
                    break;
                default:
                    break;
            }
            break;
        }
    }
}

void Map::mouse_move_event( QGraphicsSceneMouseEvent *event ) {
    if ( !player_list.empty() ) {
        for (int i = 0; i < player_list.size(); i++) {
            if ( player_list[i]->get_name() == player_name ) {
                player_list[i]->mouse_move( event -> scenePos().x() / VIEW_SCALE,
                                          event -> scenePos().y() / VIEW_SCALE );
                break;
            }
        }
    }
}

void Map::move_player(QString id , QString x , QString y) {
    for(int i = 0; i < player_list.size(); i++) {
        if( player_list[i]->get_name() == id ) {
            player_list[i]->setx(x);
            player_list[i]->sety(y);

            player_list[i]->setPos( playerList[i]->getXPosition(), player_list[i]->get_y_position());

            break;
        }
    }
}

void Map::update_player(QString id , QString s , QString r) {
    for(int i = 0; i < player_list.size(); i++) {
        if( player_list[i]->get_name() == id ) {
            player_list[i]->score_ = s.toInt();
            player_list[i]->radius_ = r.toInt();
            break;
        }
    }
}

void Map::player_eject_food(QString id) {
    for (int i = 0; i < player_list.size(); i++) {
        if( player_list[i]->getName() == id ) {
            player_list[i]->score_ -= 12;
            player_list[i]->radius_ =( ( player_list[i]->score_ / 5 ) + 17 );
            break;
        }
    }
}

void Map::remove_ejected_food(QString x , QString y) {
    for( int k = 0; k < ejectedFoodList.size(); k++ ) {
        if( ejected_food_list[k]->get_y_position() == y.toDouble() && ejected_food_list[k]->get_x_position() == x.toDouble()) {
            removeItem(ejected_food_list[k]);
            ejected_food_list.removeOne(ejected_food_list[k]);
        }
    }
}

void Map::player_lost( QString id ) {
    if( player_name == id ) {
        for(int i = 0; i < player_list.size(); i++) {
            if( player_list[i]->get_name() == id )
            {
                QMessageBox messageBox; // create a messageBox
                messageBox.setText( "You lost!" ); // assign "You lost" text to it
                messageBox.exec(); // show messageBox

                removeItem(player_list[i]);
                player_list.erase( player_list.begin() + i );

                break;
            }
        }
    } else {
        for(int i = 0; i < player_list.size(); i++) {
            if( player_list[i]->get_name() == id ) {
                removeItem(player_list[i]);
                player_list.erase( player_list.begin() + i );

                break;
            }
        }
    }
}

void Map::get_new_food( QString x, QString y, QString c ) {
    Food* food = ElementsFactory::get_instance() -> create_food( x, y, c);
    addItem( food );
    food_list.append( food );
}

void Map::food_element_is_eaten(QString xp, QString yp, QString x, QString y, QString c) {
    for( int i = 0; i < food_list.size(); i++) {
        if( food_list[i]->get_x_position() == xp.toDouble() && food_list[i]->get_y_position() == yp.toDouble() ) {
            removeItem(food_list[i]);
            food_list.removeOne(food_list[i]);
            get_new)+_food( x, y, c);

            break;
        }
    }
}

void Map::getNewPlayer(QString x, QString y, QString c , QString r, QString s, QString id) {
    Player* player = ElementsFactory::get_instance()->create_player( x, y, c, r, s, id );
    addItem(player);
    player_list.append(player);
}

void Map::get_new_ejected_food(QString x, QString y, QString cP) {
    EjectedFood* efood = ElementsFactory::get_instance()->create_ejected_food(x, y, cP);
    addItem(efood);
    ejected_food_list.append(efood);
}

Player* Map::this_player() {
    for( int k = 0; k < player_list.size(); k++ ) {
        if( player_list[k]->get_name() == player_name ) {
            return player_list[k];
        }
    }
}

void Map::playerPunish(QString n) {
    for (int k = 0; k < player_list.size(); k++) {
        if ( player_list[k]->get_name() == n ) {
            player_list[k]->score_ -= 14;
            player_list[k]->radius_ =( ( player_list[k]->score_ / 5 ) + 17 );
            break;
        }
    }
}

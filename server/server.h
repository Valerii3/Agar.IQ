#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <json.hpp>
#include <scene.h>

using json = nlohmann::json;

class server: public QTcpServer {
    Q_OBJECT

private:
    scene Game_scene;

    QVector <QTcpSocket*> sockets;

public:
    server();
    ~server();

    QTcpSocket* socket;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void sendToClient();
    void readFromClient();
    void sockDisc();
};

#endif // MYSERVER_H

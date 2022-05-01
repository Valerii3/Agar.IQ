#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <player.h>

class server: public QTcpServer {
    Q_OBJECT

private:
    QVector <QTcpSocket*> sockets;

    QJsonDocument doc;
    QJsonParseError docError;

    QVector<Player> players_data;

public:
    server();
    ~server();

    QTcpSocket* socket;
    QByteArray Data;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void sendToClient();
    void slotReadyRead();
//    void sendToClient();
    void sockDisc();
};

#endif // MYSERVER_H

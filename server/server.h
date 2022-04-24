#ifndef MYSERVER_H
#define MYSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>

class server: public QTcpServer {
    Q_OBJECT

private:
    QVector <QTcpSocket*> sockets;

public:
    server();
    ~server();

    QTcpSocket* socket;
    QByteArray Data;

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void sendToClient(QString str);
    void slotReadyRead();
//    void sendToClient();
    void sockDisc();
};

#endif // MYSERVER_H

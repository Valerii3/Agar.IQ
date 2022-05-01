#include "server.h"

server::server() {
    if(this->listen(QHostAddress::Any, 5555)) {
        qDebug() << "Listening";
    } else {
        qDebug() << "Error";
    }
}

server::~server(){}

void server::incomingConnection(qintptr socketDescriptor) {
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, &server::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &server::sockDisc);

    sockets.push_back(socket);
    players_data.push_back({"none", 0, 0, 0});

    int iterator = players_data.size() - 1;

    Data.clear();

    Data.append("{\"status\":\"connected\", \"initialization\":\"true\", \"iterator\":" + QString::number(iterator) + "}");
    socket->waitForBytesWritten(500);
    socket->write(Data);

    qDebug() << "client connected - " << socketDescriptor;
}

void server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();

    Data = socket->readAll();
    doc = QJsonDocument::fromJson(Data, &docError);

    // from client {"status":"connected", "iter":iterator, "name":"player_name", "x":x_coord, "y":y_coord, "rad":radius}
    if (docError.errorString() == "no error occurred") {
        if (doc.object().value("status") == "connected") {
            int iter = doc.object().value("iter").toInt();
            QString name = doc.object().value("name").toString();
            double x = doc.object().value("x").toDouble();
            double y = doc.object().value("y").toDouble();
            double rad = doc.object().value("rad").toDouble();

            players_data[iter] = {name, x, y, rad};

            sendToClient();
        } else {
            qDebug() << "client is disconneted";
        }
    } else {
        qDebug() << docError.errorString();
    }
}

void server::sendToClient() {
    Data.clear();

    for (auto i : players_data) {
        Data.append("{\"status\":\"connected\", \"name\":\"" + i.name + "\", \"x\":" + QString::number(i.x_coordinate)
                    + ", \"y\":" + QString::number(i.y_coordinate) + ", \"rad\":" + QString::number(i.radius) + "}");
    }

    // https://github.com/nlohmann/json

    for (int i = 0; i < sockets.size(); i++) {
        sockets[i]->write(Data);
    }
}

void server::sockDisc()
{
    qDebug() << "client disconnected - ";
    socket->deleteLater();
}

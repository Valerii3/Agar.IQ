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
//    json a = socket->readAll();

    // from client {"status":"connected", "iter":iterator, "name":"player_name", "x":x_coord, "y":y_coord, "rad":radius}
//    if (docError.errorString() == "no error occurred") {
//        if (a["status"] == "connected") {
//            int iter = a["iterator"];
//            QString name = QString::fromStdString(a["iterator"]);
//            double x = a["x"];
//            double y = a["y"];
//            double rad = a["rad"];

//            players_data[iter] = {name, x, y, rad};

//            sendToClient();
//        } else {
//            qDebug() << "client is disconneted";
//        }
//    } else {
//        qDebug() << docError.errorString();
//    }
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

    json toClient;
    for (auto i : players_data) {
//        Data.append("{\"status\":\"connected\", \"name\":\"" + i.name + "\", \"x\":" + QString::number(i.x_coordinate)
//                    + ", \"y\":" + QString::number(i.y_coordinate) + ", \"rad\":" + QString::number(i.radius) + "}");
//        json player = json::array();
//        player["status"] = "connected";
//        player["name"] = i.name.toStdString();
//        player["x"] = i.x_coordinate;
//        player["y"] = i.y_coordinate;
//        player["rad"] = i.radius;

//        toClient["players"].push_back(player);
        toClient["status"] = "connected";
        toClient["name"] = i.name.toStdString();
        toClient["x"] = i.x_coordinate;
        toClient["y"] = i.y_coordinate;
        toClient["rad"] = i.radius;
    }

    // https://github.com/nlohmann/json

    for (int i = 0; i < sockets.size(); i++) {
        sockets[i]->write(QString::fromStdString(toClient.dump()).toLatin1());
    }
}

void server::sockDisc()
{
    qDebug() << "client disconnected - ";
    socket->deleteLater();
}

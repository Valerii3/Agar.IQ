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
    Game_scene.new_player("none");

    int iterator = Game_scene.players.size() - 1;

    json initializationMessage;
    initializationMessage["status"] = "connected";
    initializationMessage["initialization"] = "yes";
    initializationMessage["iterator"] = iterator;

    socket->waitForBytesWritten(500);
    socket->write(QString::fromStdString(initializationMessage.dump()).toLatin1());

    qDebug() << "client connected - " << socketDescriptor;
}

void server::slotReadyRead()
{
    socket = (QTcpSocket*)sender();

    json fromClient;
    try {
        fromClient = json::parse(socket->readAll().toStdString());
    }  catch (json::exception& e) {
        qDebug() << e.what() << '\n';
    }


    // from client {"status":"connected", "iter":iterator, "name":"player_name", "x":x_coord, "y":y_coord, "rad":radius}
    if (fromClient["status"] == "connected") {
        int iter = fromClient["iter"];
        QString name = QString::fromStdString(fromClient["name"]);
        double x = fromClient["x"];
        double y = fromClient["y"];
        double rad = fromClient["rad"];

        QVector<int> eaten_foods = fromClient["eaten_foods"];
        QVector<int> eaten_answers = fromClient["eaten_answers"];

        for (auto i : eaten_foods) {
            Game_scene.new_food(i);
        }

        for (auto i : eaten_answers) {
            Game_scene.new_answer(i);
        }

        Game_scene.update_player(iter, name, x, y, rad);

        sendToClient();
    } else {
        qDebug() << "client is disconnected";
    }
}

void to_json(json& j, const Player& p)
{
    j = {{"name", p.name}, {"x", p.x_coordinate}, {"y", p.y_coordinate}, {"rad", p.radius}};
}

void to_json(json& j, const Entity& p)
{
    j = {{"x", p.x_coordinate}, {"y", p.y_coordinate}};
}

void server::sendToClient() {
    json toClient;

    toClient.clear();

    toClient["players"] = Game_scene.get_players();
    toClient["answers"] = Game_scene.get_answers();
    toClient["foods"] = Game_scene.get_foods();
    toClient["status"] = "connected";

    for (int i = 0; i < sockets.size(); i++) {
        sockets[i]->write(QString::fromStdString(toClient.dump()).toLatin1());
    }
    toClient.clear();
}

void server::sockDisc()
{
    qDebug() << "client disconnected - ";
    socket->deleteLater();
}

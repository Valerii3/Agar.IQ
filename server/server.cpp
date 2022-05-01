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

        Game_scene.update_player(iter, name, x, y, rad);

        sendToClient();
    } else {
        qDebug() << "client is disconneted";
    }
}

struct player {
    std::string status;
    std::string name;
    double x;
    double y;
    double rad;
};

void to_json(json& j, const player& p)
{
    j = {{"status", p.status}, {"name", p.name}, {"x", p.x}, {"y", p.y}, {"rad", p.rad}};
}

void server::sendToClient() {
    json toClient;

    std::vector<player> players_data;

    for (auto i : Game_scene.players) {
        player p{"connected", i.name.toStdString(), i.x_coordinate, i.y_coordinate, i.radius};
//        toClient["status"] = "connected";
//        toClient["name"] = i.name.toStdString();
//        toClient["x"] = i.x_coordinate;
//        toClient["y"] = i.y_coordinate;
//        toClient["rad"] = i.radius;
        players_data.push_back(p);
    }

    toClient = players_data;

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

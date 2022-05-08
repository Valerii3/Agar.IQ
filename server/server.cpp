#include "server.h"

server::server() {
    if(this->listen(QHostAddress::Any, 5555)) {
        qDebug() << "Listening";
    } else {
        qDebug() << "Error";
    }
}

server::~server(){}

void to_json(json& j, const Player& p)
{
    j = {{"name", p.player_name}, {"x", p.x_coordinate}, {"y", p.y_coordinate},
         {"rad", p.radius}, {"score", p.score}, {"is_correct", p.is_correct}};
}

void to_json(json& j, const Answer& p)
{
    j = {{"x", p.x_coordinate}, {"y", p.y_coordinate}, {"number", p.get_number()}};
}

void to_json(json& j, const Food& p)
{
    j = {{"x", p.x_coordinate}, {"y", p.y_coordinate}, {"red_color", p.red_color},
         {"green_color", p.green_color}, {"blue_color", p.blue_color}};
}

void server::incomingConnection(qintptr socketDescriptor) {
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, &server::readFromClient);
    connect(socket, &QTcpSocket::disconnected, this, &server::sockDisc);

    sockets.push_back(socket);
    Game_scene.new_player("none");

    int newClientID = Game_scene.players.size() - 1;

    // first message from server is reply to connection and
    //       client's id (his number in players_data):
    //       {"status":"connected", "initialization":"yes",
    //       "id":newClientID}

    json initializationMessage;
    initializationMessage["status"] = "connected";
    initializationMessage["initialization"] = "yes";
    initializationMessage["id"] = newClientID;

    socket->waitForBytesWritten(500);
    socket->write(QString::fromStdString(initializationMessage.dump()).toLatin1());

    qDebug() << "client connected - " << socketDescriptor;
}

void server::readFromClient()
{
    socket = (QTcpSocket*)sender();

    json fromClient;
    try {
        fromClient = json::parse(socket->readAll().toStdString());
    }  catch (json::exception& e) {
        qDebug() << e.what() << '\n';
    }

    // every message from client to server is player data:
    //      { "status":"connected", "name":player_name, "id":clientID,
    //       "angle":player_angle }

    // if client is first additionally get settings:
    //       "bits":bits, "operandsCount":operandsCount, "operands":operands

    if (fromClient["status"] == "connected") {
        int clientID = fromClient["id"];
        QString name = QString::fromStdString(fromClient["name"]);
        double player_angle = fromClient["angle"];

        if (clientID == 0) {
            Game_scene.bits = fromClient["bits"];
            Game_scene.operandsCount = fromClient["operandsCount"];
            Game_scene.operands = fromClient["operands"];
        }

        Game_scene.update_player(clientID, name, player_angle);

        Game_scene.update(clientID);

        sendToClient();
    } else {
        qDebug() << "client is disconnected";
    }
}

void server::sendToClient() {
    json toClient;

    // every next message is all scene data, include players, answers
    //       and dots datas, and example:
    //       {"status":"connected", "players":[ players data ],
    //        "answers":[ answers data ], "food":[ food data ],
    //        "expr":example }

    toClient["players"] = Game_scene.get_players();
    toClient["answers"] = Game_scene.get_answers();
    toClient["food"] = Game_scene.get_food();
    toClient["status"] = "connected";

    toClient["expr"] = Game_scene.expr;

    for (int i = 0; i < sockets.size(); i++) {
        sockets[i]->write(QString::fromStdString(toClient.dump()).toLatin1());
    }
}

void server::sockDisc()
{
    qDebug() << "client disconnected - ";
}

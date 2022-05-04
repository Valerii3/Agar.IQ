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
    //      { "status":"connected", "name":playerName, "id":clientID,
    //      "x":x_coorditate, "y":y_coordinate, "rad":radius,
    //      "eatenFood":[ eaten_food ], "eatenAnswers":[ eatenAnswers ] }

    if (fromClient["status"] == "connected") {
        int clientID = fromClient["id"];
        QString name = QString::fromStdString(fromClient["name"]);
        double x = fromClient["x"];
        double y = fromClient["y"];
        double rad = fromClient["rad"];

        QVector<int> eaten_food = fromClient["eaten_food"];
        QVector<int> eaten_answers = fromClient["eaten_answers"];

        if (!eaten_food.empty()) {
            for (auto i : eaten_food) {
                Game_scene.new_food(i);
            }
        }

        if (!eaten_answers.empty()) {
            for (auto i : eaten_answers) {
                Game_scene.new_answer(i);
            }
        }

        Game_scene.update_player(clientID, name, x, y, rad);

        sendToClient();
    } else {
        qDebug() << "client is disconnected";
    }
}

void to_json(json& j, const Player& p)
{
    j = {{"name", p.player_name}, {"x", p.x_coordinate}, {"y", p.y_coordinate}, {"rad", p.radius}};
}

void to_json(json& j, const Entity& p)
{
    j = {{"x", p.x_coordinate}, {"y", p.y_coordinate}};
}

void server::sendToClient() {
    json toClient;

    // every next message is scene data, include players data,
    //       answer-dots data and dots data:
    //       {"status":"connected", "players":[ players data ],
    //        "answers":[ answers data ], "food":[ food data ]}

    toClient["players"] = Game_scene.get_players();
    toClient["answers"] = Game_scene.get_answers();
    toClient["foods"] = Game_scene.get_food();
    toClient["status"] = "connected";

    for (int i = 0; i < sockets.size(); i++) {
        sockets[i]->write(QString::fromStdString(toClient.dump()).toLatin1());
    }
}

void server::sockDisc()
{
    qDebug() << "client disconnected - ";
}

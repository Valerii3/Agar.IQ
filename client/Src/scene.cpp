#include "../Include/scene.h"
#include "ui_scene.h"
#include "../Include/settingswindow.h"

Scene::Scene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scene)
{
    socket = new QTcpSocket;

    connect(socket, &QTcpSocket::readyRead, this, &Scene::readFromServer);
    connect(socket, &QTcpSocket::disconnected, this, &QTcpSocket::deleteLater);

    socket->connectToHost("127.0.0.1", 5555);
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);
}

Scene::~Scene()
{
    delete ui;
    emit signalQuitGame(true);
    workerThread.quit();
    workerThread.wait();
    delete worker;
}

void Scene::slotResultReady(){
    sendToServer();
}

bool Scene::in_bounds(Entity dot) {
    if (abs(worker->players_data[clientID].get_x_position() - dot.get_x_position()) > 950) {
        return false;
    }

    if (abs(worker->players_data[clientID].get_y_position() - dot.get_y_position()) > 450) {
        return false;
    }

    return true;
}

void Scene::paintEvent(QPaintEvent *event) {
    Q_UNUSED(event);
    QPainter painter(this);
    QFont fnt;
    fnt.setPixelSize(35);
    painter.setFont(fnt);
    painter.setPen(QPen(Qt::black,1,Qt::SolidLine,Qt::FlatCap));

    if (isMenu){
        return;
    }

    for (auto it : worker->answers_data) {
        if (in_bounds(it)) {

            painter.setBrush(QBrush(it.color, Qt::SolidPattern));
            double new_x = center_x + it.get_x_position() - worker->players_data[clientID].get_x_position();
            double new_y = center_y - it.get_y_position() + worker->players_data[clientID].get_y_position();

            painter.drawEllipse(QPointF(new_x, new_y), 2*it.get_radius(), 2*it.get_radius());
            painter.drawText(QPoint(new_x - it.get_radius(), new_y + it.get_radius()/2), QString::number(it.get_number()));
        }
    }

    for (auto it : worker->food_data) {
        if (in_bounds(it)) {
            painter.setBrush(QBrush(it.color, Qt::SolidPattern));

            double new_x = center_x + it.get_x_position() - worker->players_data[clientID].get_x_position();
            double new_y = center_y - it.get_y_position() + worker->players_data[clientID].get_y_position();

            painter.drawEllipse(QPointF(new_x, new_y), 2*it.get_radius(), 2*it.get_radius());
        }
    }

    for (int i = 0; i < worker->players_data.size(); i++) {
        if (i == clientID) {
            painter.setBrush(QBrush(worker->players_data[i].color, Qt::SolidPattern));
            painter.drawEllipse(QPointF(center_x, center_y), 2*worker->players_data[i].get_radius(), 2*worker->players_data[i].get_radius());

            fnt.setPixelSize(20);
            painter.setFont(fnt);
            painter.drawText(center_x - worker->players_data[i].get_radius(), center_y + worker->players_data[i].get_radius()/4,
                             QString::fromStdString(worker->players_data[i].get_name()));
        } else {
            if (in_bounds(worker->players_data[i])) {
                double new_x = center_x + worker->players_data[i].get_x_position() - worker->players_data[clientID].get_x_position();
                double new_y = center_y + worker->players_data[i].get_y_position() - worker->players_data[clientID].get_y_position();

                painter.setBrush(QBrush(worker->players_data[i].color, Qt::SolidPattern));
                painter.drawEllipse(QPointF(new_x, new_y), 2*worker->players_data[i].get_radius(), 2*worker->players_data[i].get_radius());

                fnt.setPixelSize(20);
                painter.setFont(fnt);
                painter.drawText(center_x - worker->players_data[i].get_radius(), center_y + worker->players_data[i].get_radius()/4,
                                 QString::fromStdString(worker->players_data[i].get_name()));
            }
        }
    }

    fnt.setPixelSize(40);
    painter.setFont(fnt);
    painter.drawText(QPoint(1700,40), worker->text);
    painter.drawText(QPoint(1820,40), QString::number(worker->score));
    painter.drawText(QPoint(1700,80), worker->is_correct);
    painter.drawText(QPoint(800,40), QString::fromLocal8Bit(worker->expr.c_str()));
}

void Scene::mouseMoveEvent(QMouseEvent *event)
{
    worker->player.player_angle = atan2( - event->y() + center_y,
                                         event->x() - center_x);
}

void Scene::startGame()
{

    worker = new Worker;
    worker->moveToThread(&workerThread);
    connect(this, &Scene::signalQuitGame, worker, &Worker::slotQuitGame);
    connect(worker, &Worker::signalResultReady, this, &Scene::slotResultReady);
    connect(this, &Scene::startWork, worker, &Worker::doWork);
    connect(worker, &Worker::signalGameFinish, this, &Scene::slotGameFinish);
    workerThread.start();
    emit startWork();
    isMenu = false;
}

void Scene::slotGameFinish(){
    qDebug() << "finish";
    workerThread.quit();
    workerThread.wait();
    isMenu = true;
}

void Scene::on_pushButton_clicked()
{
    this->close();
}

void Scene::sendToServer()
{
    json toServer;

    // every message from client to server is player data and settings:
    //      { "status":"connected", "name":player_name,
    //       "id":clientID, "angle":player_angle, "bits":bits,
    //       "operandsCount":operandsCount, "operands" }

    toServer["status"] = "connected";
    toServer["name"] = name.toStdString();
    toServer["id"] = clientID;
    toServer["angle"] = worker->player.player_angle;

    toServer["blue_color"] = (int) (worker->player.color.blueF() * 255.0);
    toServer["green_color"] = (int) (worker->player.color.greenF() * 255.0);
    toServer["red_color"] = (int) (worker->player.color.redF() * 255.0);

    toServer["bits"] = worker->bits;
    toServer["operandsCount"] = worker->operandsCount;
    toServer["operands"] = worker->operands;

    socket->write(QString::fromStdString(toServer.dump()).toLatin1());
    socket->waitForBytesWritten(20000);
}

void Scene::readFromServer()
{
    socket = (QTcpSocket*)sender();

    json fromServer;
    try {
        fromServer = json::parse(socket->readAll().toStdString());
    }  catch (json::exception& e) {
        qDebug() << e.what() << '\n';
    }

    // first message from server is reply to connection and
    //       client's id (his number in players_data):
    //       {"status":"connected", "initialization":"yes", "id":clientID}

    // every next message is all scene data, include players, answers
    //       and dots datas, and example:
    //       {"status":"connected", "players":[ players data ],
    //        "answers":[ answers data ], "food":[ food data ],
    //        "expr":example }

    if (fromServer["status"] == "connected" && fromServer["initialization"] == "yes") {
        qDebug() << "connected to server";

        clientID = fromServer["id"];

    } else if (fromServer["status"] == "connected") {

        worker->answers_data.clear();
        worker->players_data.clear();

        int player_iter = 0;

        for (auto player : fromServer["players"]) {
            QString name = QString::fromStdString(player["name"]);
            double x = player["x"];
            double y = player["y"];
            double rad = player["rad"];
            int score = player["score"];

            int blue_color = player["blue_color"];
            int green_color = player["green_color"];
            int red_color = player["red_color"];

            QString is_correct = QString::fromStdString(player["is_correct"]);

            worker->players_data.push_back({name, x, y, rad, score, is_correct});

            worker->players_data[player_iter].color = QColor(red_color, green_color, blue_color);

            player_iter++;
        }

        for (auto answer : fromServer["answers"]) {
            double x = answer["x"];
            double y = answer["y"];
            int number = answer["number"];

            worker->answers_data.push_back({x, y, number});
        }

        if (fromServer["food_status"] == "full") {
            for (auto food : fromServer["food"]) {
                double x = food["x"];
                double y = food["y"];
                int red = food["red_color"];
                int green = food["green_color"];
                int blue = food["blue_color"];

                worker->food_data.push_back({x, y, red, green, blue});
            }
        } else {
            for (auto food : fromServer["food"]) {
                int food_iter = food["food_iter"];
                double x = food["x"];
                double y = food["y"];
                int red = food["red_color"];
                int green = food["green_color"];
                int blue = food["blue_color"];

                worker->food_data[food_iter] = {x, y, red, green, blue};
            }
        }

        worker->is_correct = worker->players_data[clientID].is_correct;
        worker->score = worker->players_data[clientID].score;
        worker->expr = fromServer["expr"];

        repaint();
    } else {
        qDebug() << "not connected to server";
    }
}

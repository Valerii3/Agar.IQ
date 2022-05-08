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

void Scene::paintEvent(QPaintEvent *event){
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
        painter.setBrush(QBrush(it.color, Qt::SolidPattern));
        painter.drawEllipse(QPointF(it.get_x_position(), it.get_y_position()), 2*it.get_radius(), 2*it.get_radius());
        painter.drawText(QPoint(it.get_x_position() - it.get_radius(),it.get_y_position() + it.get_radius()/2), QString::number(it.get_number()));   // some changes with radius
    }

    for (auto it : worker->food_data) {
        painter.setBrush(QBrush(it.color, Qt::SolidPattern));
        painter.drawEllipse(QPointF(it.get_x_position(), it.get_y_position()), 2*it.get_radius(), 2*it.get_radius());
    }

    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));  // instead green player.color

    for (auto player : worker->players_data) {

        painter.drawEllipse(QPointF(player.get_x_position(), player.get_y_position()), 2*player.get_radius(), 2*player.get_radius());

        fnt.setPixelSize(20);
        painter.setFont(fnt);
        painter.drawText(player.get_x_position() - player.get_radius(), player.get_y_position() + player.get_radius()/4, QString::fromStdString(player.get_name()));
    }

    fnt.setPixelSize(40);
    painter.setFont(fnt);
    painter.drawText(QPoint(1700,40), worker->text);
    painter.drawText(QPoint(1820,40), QString::number(worker->score));
    painter.drawText(QPoint(1700,80), worker->is_correct);
    painter.drawText(QPoint(800,40), QString::fromLocal8Bit(worker->expr.c_str()));
}

void Scene::keyPressEvent(QKeyEvent *event){
    if (!isMenu && event->type() == QEvent::KeyPress){
        QKeyEvent *key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_W){
              worker->player.player_speed = -5;
        }
    }
}

void Scene::keyReleaseEvent(QKeyEvent *event){
    if (!isMenu && event->type() == QEvent::KeyRelease){
        QKeyEvent *key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_W){
            worker->player.player_speed = 0;
        }
    }
}

void Scene::mouseMoveEvent(QMouseEvent *event)
{
    worker->player.player_angle = atan2(event->y() - worker->players_data[clientID].get_y_position(),
                                        event->x() - worker->players_data[clientID].get_x_position());
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

    // every message from client to server is player data:
    //      { "status":"connected", "name":playerName, "id":clientID,
    //      "x":x_coorditate, "y":y_coordinate, "rad":radius,
    //      "eatenFood":[ eaten_food ], "eatenAnswers":[ eatenAnswers ] }

    toServer["status"] = "connected";
    toServer["name"] = name.toStdString();
    toServer["id"] = clientID;
    toServer["angle"] = worker->player.player_angle;

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
    //       {"status":"connected", "initialization":"yes", "id":clientId}

    // every next message is scene data, include players data,
    //       answer-dots data and dots data:
    //       {"status":"connected", "players":[ players data ],
    //        "answers":[ answers data ], "food":[ food data ]}

    if (fromServer["status"] == "connected" && fromServer["initialization"] == "yes") {
        qDebug() << "connected to server";

        clientID = fromServer["id"];

    } else if (fromServer["status"] == "connected") {

//        qDebug() << "read players";
//        qDebug() << QString::fromStdString(fromServer.dump());

        worker->answers_data.clear();
        worker->food_data.clear();
        worker->players_data.clear();

        for (auto player : fromServer["players"]) {
            QString name = QString::fromStdString(player["name"]);
            double x = player["x"];
            double y = player["y"];
            double rad = player["rad"];
            int score = player["score"];
            QString is_correct = QString::fromStdString(player["is_correct"]);

            worker->players_data.push_back({name, x, y, rad, score, is_correct});
//            qDebug() << name << ' ' << x << ' ' << y << ' ' << rad;
        }

        for (auto answer : fromServer["answers"]) {
            double x = answer["x"];
            double y = answer["y"];
            int number = answer["number"];

            worker->answers_data.push_back({x, y, number});
//            qDebug() << "answer" << ' ' << x << ' ' << y << ' ' << number;
        }

        for (auto food : fromServer["food"]) {
            double x = food["x"];
            double y = food["y"];
            int red = food["red_color"];
            int green = food["green_color"];
            int blue = food["blue_color"];

            worker->food_data.push_back({x, y, red, green, blue});
//            qDebug() << "food" << ' ' << x << ' ' << y;
        }

        worker->is_correct = worker->players_data[clientID].is_correct;
        worker->score = worker->players_data[clientID].score;
        worker->expr = fromServer["expr"];

//        qDebug() << QString::fromStdString(worker->expr) << ' ' << worker->generator;

        repaint();
    } else {
        qDebug() << "not connected to server";
    }
}

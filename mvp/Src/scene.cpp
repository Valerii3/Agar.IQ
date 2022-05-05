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
    repaint();
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

//    if (initialization) {
//        for (auto it : initialization_answers) {
//            painter.setBrush(QBrush(it.color, Qt::SolidPattern));
//            painter.drawEllipse(QPointF(it.get_x_position(), it.get_y_position()), 2*it.get_radius(), 2*it.get_radius());
//            painter.drawText(QPoint(it.get_x_position() - it.get_radius(),it.get_y_position() + it.get_radius()/2), QString::number(it.get_number()));   // some changes with radius
//        }

//        for (auto it : initialization_food) {
//            painter.setBrush(QBrush(it.color, Qt::SolidPattern));
//            painter.drawEllipse(QPointF(it.get_x_position(), it.get_y_position()), 2*it.get_radius(), 2*it.get_radius());
//        }
//    }

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

    emit sendToServer();
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
    worker->player.player_angle = atan2(event->y() - worker->player.get_y_position(), event->x() - worker->player.get_x_position());
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
    toServer["x"] = worker->player.get_x_position();
    toServer["y"] = worker->player.get_y_position();
    toServer["rad"] = worker->player.get_radius();

    toServer["eaten_food"] = worker->eaten_food;
    toServer["eaten_answers"] = worker->eaten_answers;

    worker->eaten_answers.clear();
    worker->eaten_food.clear();

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

//        worker->answers_data.clear();
//        worker->food_data.clear();

//        for (auto answer : fromServer["answers"]) {
//            double x = answer["x"];
//            double y = answer["y"];

//            initialization_answers.push_back(Answer(x, y));
//            qDebug() << "answer" << ' ' << x << ' ' << y;
//        }

//        for (auto food : fromServer["food"]) {
//            double x = food["x"];
//            double y = food["y"];

//            initialization_food.push_back(Food(x, y));
//            qDebug() << "food" << ' ' << x << ' ' << y;
//        }

        // ещё не созданы эти объекты, переделать на статик методы

    } else if (fromServer["status"] == "connected") {

        worker->players_data.clear();

        qDebug() << "read players";
//        qDebug() << QString::fromStdString(fromServer.dump());

        for (auto player : fromServer["players"]) {
            QString name = QString::fromStdString(player["name"]);
            double x = player["x"];
            double y = player["y"];
            double rad = player["rad"];

            worker->players_data.push_back({name, x, y, rad});
//            qDebug() << name << ' ' << x << ' ' << y << ' ' << rad;
        }

        for (auto updated_answer : fromServer["updated_answers"]) {
            int id = updated_answer["id"];
            double x = updated_answer["x"];
            double y = updated_answer["y"];

            worker->answers_data[id] = {x, y};
            worker->update_answer(id);
//            worker->answers_data.push_back({x, y});
//            qDebug() << "answer" << ' ' << x << ' ' << y;
        }

        for (auto updated_food : fromServer["updated_food"]) {
            int id = updated_food["id"];
            double x = updated_food["x"];
            double y = updated_food["y"];

            worker->food_data[id] = {x, y};

//            worker->answers_data.push_back({x, y});
//            qDebug() << "answer" << ' ' << x << ' ' << y;
        }

    } else {
        qDebug() << "not connected to server";
    }
}

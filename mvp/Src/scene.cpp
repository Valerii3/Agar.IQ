#include "../Include/scene.h"
#include "ui_scene.h"
#include "../Include/settingswindow.h"

Scene::Scene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scene)
{
    socket = new QTcpSocket;

    connect(socket, &QTcpSocket::readyRead, this, &Scene::slotReadyRead);
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
    for (auto it : worker->answers_data) {
        painter.setBrush(QBrush(it.color, Qt::SolidPattern));
        painter.drawEllipse(QPointF(it.get_x_position(), it.get_y_position()), 2*it.get_radius(), 2*it.get_radius());
        painter.drawText(QPoint(it.get_x_position() - it.get_radius(),it.get_y_position() + it.get_radius()/2), QString::number(it.get_number()));   // some changes with radius
    }
    for (auto it : worker->foods_data) {
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
              worker->player.speed = -5;
        }
    }
}

void Scene::keyReleaseEvent(QKeyEvent *event){
    if (!isMenu && event->type() == QEvent::KeyRelease){
        QKeyEvent *key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_W){
            worker->player.speed = 0;
        }
    }
}

void Scene::mouseMoveEvent(QMouseEvent *event)
{
    worker->player.angle = atan2(event->y() - worker->player.get_y_position(), event->x() - worker->player.get_x_position());
    //qDebug() << worker->entities[0].angle;
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
    toServer["status"] = "connected";
    toServer["name"] = name.toStdString();
    toServer["iter"] = server_iterator;
    toServer["x"] = worker->player.get_x_position();
    toServer["y"] = worker->player.get_y_position();
    toServer["rad"] = worker->player.get_radius();

    toServer["eaten_foods"] = worker->eaten_foods;
    toServer["eaten_answers"] = worker->eaten_answers;

    worker->eaten_answers.clear();
    worker->eaten_foods.clear();

    socket->write(QString::fromStdString(toServer.dump()).toLatin1());
    socket->waitForBytesWritten(20000);
}

void Scene::slotReadyRead()
{
    socket = (QTcpSocket*)sender();

    json fromServer;
    try {
        fromServer = json::parse(socket->readAll().toStdString());
    }  catch (json::exception& e) {
        qDebug() << e.what() << '\n';
    }

    // from client {"status":"connected", "iter":iterator, "name":"player_name", "x":x_coord, "y":y_coord, "rad":radius}
    if (fromServer["status"] == "connected" && fromServer["initialization"] == "yes") {
        qDebug() << "connected to server";
        server_iterator = fromServer["iterator"];
    } else if (fromServer["status"] == "connected") {

        worker->players_data.clear();
        worker->answers_data.clear();
        worker->foods_data.clear();

        qDebug() << "read players";

        for (auto player : fromServer["players"]) {
            QString name = QString::fromStdString(player["name"]);
            double x = player["x"];
            double y = player["y"];
            double rad = player["rad"];

            worker->players_data.push_back({name, x, y, rad});
            qDebug() << name << ' ' << x << ' ' << y << ' ' << rad;
        }

        for (auto answer : fromServer["answers"]) {
            double x = answer["x"];
            double y = answer["y"];

            worker->answers_data.push_back({x, y});
            qDebug() << "answer" << ' ' << x << ' ' << y;
        }

        for (auto food : fromServer["foods"]) {
            double x = food["x"];
            double y = food["y"];

            worker->foods_data.push_back({x, y});
            qDebug() << "food" << ' ' << x << ' ' << y;
        }

    } else {
        qDebug() << "not connected to server";
    }
}

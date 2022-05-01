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
    for (auto it : worker->answers) {
        painter.setBrush(QBrush(it.color, Qt::SolidPattern));
        painter.drawEllipse(QPointF(it.get_x_position(), it.get_y_position()), 2*it.get_radius(), 2*it.get_radius());
        painter.drawText(QPoint(it.get_x_position() - it.get_radius(),it.get_y_position() + it.get_radius()/2), QString::number(it.get_number()));   // some changes with radius
    }
    for (auto it : worker->food) {
        painter.setBrush(QBrush(it.color, Qt::SolidPattern));
        painter.drawEllipse(QPointF(it.get_x_position(), it.get_y_position()), 2*it.get_radius(), 2*it.get_radius());
    }


    painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));  // instead green player.color

    for (auto player : players_data) {

        painter.drawEllipse(QPointF(player.get_x_position(), player.get_y_position()), 2*player.get_radius(), 2*player.get_radius());

//    painter.drawEllipse(QPointF(worker->player.get_x_position(), worker->player.get_y_position()), 2*worker->player.get_radius(), 2*worker->player.get_radius());


        fnt.setPixelSize(20);
        painter.setFont(fnt);
        painter.drawText(player.get_x_position() - player.get_radius(), player.get_y_position() + player.get_radius()/4, QString::fromStdString(player.get_name()));
//    painter.drawText(worker->player.get_x_position() - worker->player.get_radius(), worker->player.get_y_position() + worker->player.get_radius()/4, QString::fromStdString(worker->player.get_name()));
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
    Data.clear();
    Data.append("{\"status\":\"connected\", \"name\":\"" + name + "\", \"iterator\":" + QString::number(server_iterator) + ", \"x\":" + QString::number(worker->player.get_x_position())
            + ", \"y\":" + QString::number(worker->player.get_y_position()) + ", \"rad\":" + QString::number(worker->player.get_radius()) + "}");
    socket->write(Data);
    socket->waitForBytesWritten(20000);
}

void Scene::slotReadyRead()
{
    socket = (QTcpSocket*)sender();


    Data = socket->readAll();

    doc = QJsonDocument::fromJson(Data, &docError);

    if (docError.errorString() == "no error occurred") {
        if (doc.object().value("status").toString() == "connected" && doc.object().value("initialization").toString() == "yes") {
            qDebug() << "connected to server";
            server_iterator = doc.object().value("iterator").toInt();
        } else if (doc.object().value("status").toString() == "connected") {
            QString name = "stupid";
            double x = 0;
            double y = 0;
            double rad = 0;

            name = doc.object().value("name").toString();
            x = doc.object().value("x").toDouble();
            y = doc.object().value("y").toDouble();
            rad = doc.object().value("rad").toDouble();

            players_data.clear();
            players_data.push_back({name, x, y, rad});

            qDebug() << name << ' ' << x << ' ' << y << ' ' << rad;
        } else {
            qDebug() << "not connected to server";
        }
    } else {
        qDebug() << "format error";
    }
}

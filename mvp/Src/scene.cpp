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
    emit sendToServer(name);
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

    QString name = row_str.section('|', 0, 0);
    double x = row_str.section('|', 1, 1).toDouble();
    double y = row_str.section('|', 2, 2).toDouble();
    double rad = row_str.section('|', 3, 3).toDouble();
    qDebug() << name << ' ' << x << ' ' << y << ' ' << rad;
    painter.drawEllipse(QPointF(x, y), 2*rad, 2*rad);


//    painter.drawEllipse(QPointF(worker->player.get_x_position(), worker->player.get_y_position()), 2*worker->player.get_radius(), 2*worker->player.get_radius());


    fnt.setPixelSize(20);
    painter.setFont(fnt);
    painter.drawText(x - rad, y + rad/4, name);
//    painter.drawText(worker->player.get_x_position() - worker->player.get_radius(), worker->player.get_y_position() + worker->player.get_radius()/4, QString::fromStdString(worker->player.get_name()));

    fnt.setPixelSize(40);
    painter.setFont(fnt);
    painter.drawText(QPoint(1700,40), worker->text);
    painter.drawText(QPoint(1820,40), QString::number(worker->score));
    painter.drawText(QPoint(1700,80), worker->is_correct);
    painter.drawText(QPoint(800,40), QString::fromLocal8Bit(worker->expr.c_str()));

    emit sendToServer(name + "|" + QString::number(worker->player.get_x_position()) + "|" + QString::number(worker->player.get_y_position()) + "|" + QString::number(worker->player.get_radius()));

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
/*
void Scene::on_settingsButton_clicked() {
    SettingsWindow *sw = new SettingsWindow();
    sw->show();
}
*/
void Scene::slotGameFinish(){
    qDebug() << "finish";
    workerThread.quit();
    workerThread.wait();
 //   delete worker;
 //   ui->startGameButton->show();
 //   ui->settingsButton->show();
    isMenu = true;
}

void Scene::on_pushButton_clicked()
{
 /*   this->close();
    emit first();
    delete ui;                    // добавил чуть из декструктора сцены
    emit signalQuitGame(true);
    delete worker; */            // maybe it will be return to the main window
    this->close();

}

void Scene::sendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << str;
    socket->write(Data);
}

void Scene::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    if (in.status() == QDataStream::Ok) {
        QString str;
        in >> str;
        row_str = str;
    } else {
        qDebug() << "error";
    }
}

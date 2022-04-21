#include "Include/mainwindow.h"
#include "ui_mainwindow.h"
#include "Include/settingswindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    socket = new QTcpSocket;

    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &QTcpSocket::deleteLater);

    scene = new Scene;
    connect(scene, &Scene::first, this, &MainWindow::show);
    connect(this, &MainWindow::signalStartGame, scene, &Scene::startGame);
}

MainWindow::~MainWindow() {
    delete scene;
    delete ui;
}


void MainWindow::on_startButton_clicked()
{
    socket->connectToHost("127.0.0.1", 5555);
    scene->showFullScreen();
    this->close();
    emit signalStartGame();
}


void MainWindow::on_settingsButton_clicked()
{
    SettingsWindow *sw = new SettingsWindow();
    sw->showFullScreen();
}



void MainWindow::on_greenButton_clicked()
{
    repaint();
}


void MainWindow::on_yellowButton_clicked()
{
    repaint();
}


void MainWindow::on_purpleButton_clicked()
{
    repaint();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));


    if(ui->greenButton->isChecked()){

        painter.setBrush(QBrush(Qt::green, Qt::SolidPattern));
        painter.drawEllipse(1455, 375, 200, 200);
    } else if(ui->yellowButton->isChecked()){

        painter.setBrush(QBrush(Qt::yellow, Qt::SolidPattern));
        painter.drawEllipse(1455, 375, 200, 200);
    } else if(ui->purpleButton->isChecked()){

        painter.setBrush(QBrush(Qt::magenta, Qt::SolidPattern));
        painter.drawEllipse(1455, 375, 200, 200);
    } else {

        painter.setBrush(QBrush(Qt::white, Qt::SolidPattern));
        painter.drawEllipse(1455, 375, 200, 200);
    }
}

void MainWindow::on_closeButton_clicked()
{
    this->close();
}

void MainWindow::sendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out << str;
    socket->write(Data);
}

void MainWindow::slotReadyRead()
{
    socket = (QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    if (in.status() == QDataStream::Ok) {
//        qDebug() << "read...";
//        QString str;
//        in >> str;
//        qDebug() << str;
    } else {
//        qDebug() << "error";
    }
}


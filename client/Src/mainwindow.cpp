#include "Include/mainwindow.h"
#include "ui_mainwindow.h"
#include "Include/settingswindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    scene = new Scene;
    connect(scene, &Scene::first, this, &MainWindow::show);
    connect(this, &MainWindow::signalStartGame, scene, &Scene::startGame);
    connect(this, &MainWindow::signalSendColor, scene, &Scene::slotGetColor);
    connect(this, &MainWindow::signalSendName, scene, &Scene::slotGetName);
    connect(scene, &Scene::signalSendToDB, this, &MainWindow::slotMaxScore);


    db = QSqlDatabase::addDatabase("QSQLITE");                         // insert (maybe send it to another file)
    db.setDatabaseName("C:/Project_C++/Agar.IQ/mvp/db.sqlite");  // здесь свой путь поставь где находится
    if (!db.open()){
        qDebug() << "Error";
    } else {
        qDebug() << "Success";
    }

    model = new QSqlTableModel(this,db);   // табличка рекордов
    model->setTable("record");
    model->select();
    ui->tableView->setModel(model);

}

MainWindow::~MainWindow() {
    delete scene;
    delete ui;
}


void MainWindow::on_startButton_clicked()
{
    scene->show();
    this->close();
    emit signalStartGame();
    emit signalSendColor(color);
    emit signalSendName(username);
}


void MainWindow::on_settingsButton_clicked()
{
    SettingsWindow *sw = new SettingsWindow();
    sw->show();
}



void MainWindow::on_greenButton_clicked()
{
    color = Qt::green;
    repaint();
}


void MainWindow::on_yellowButton_clicked()
{
    color = Qt::yellow;
    repaint();
}


void MainWindow::on_purpleButton_clicked()
{
    color = Qt::magenta;
    repaint();
}


void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);

    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::FlatCap));
    painter.setBrush(QBrush(color, Qt::SolidPattern));
    painter.drawEllipse(1455, 375, 200, 200);

}

void MainWindow::on_closeButton_clicked()
{
    this->close();
}

void MainWindow::slotNameLog(QString _name){
    username = _name;
    ui->label->setText("Добро пожаловать, " + username);
}

void MainWindow::slotMaxScore(int _score){
    QSqlQuery qry;

    qry.prepare("INSERT INTO record ("
                "Nickname,"
                "Points)"
                "VALUES (?,?);");
    qry.addBindValue(username);
    qry.addBindValue(_score);


    if (!qry.exec()){
        qDebug() << "error add";
    } else {
        qDebug() << "succeed adding";
    }

}

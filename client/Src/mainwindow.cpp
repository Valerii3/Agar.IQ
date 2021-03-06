#include "../include/mainwindow.h"
#include "ui_mainwindow.h"
#include "Include/settingswindow.h"
#include "../Include/login.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QPixmap bkgnd(":/Data/Data/Screenshot_103.jpg");
  //  bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    scene = new Scene;
    connect(scene, &Scene::first, this, &MainWindow::show);
    connect(this, &MainWindow::signalStartGame, scene, &Scene::startGame);
    connect(this, &MainWindow::signalSendColor, scene, &Scene::slotGetColor);
    connect(this, &MainWindow::signalSendName, scene, &Scene::slotGetName);
    connect(scene, &Scene::signalSendToDB, this, &MainWindow::slotMaxScore);


    db = QSqlDatabase::addDatabase("QSQLITE");                         // insert (maybe send it to another file)
    db.setDatabaseName("C:/ProjectDB/Agar.IQ/client/db.sqlite");  // здесь свой путь поставь где находится
    if (!db.open()){
        qDebug() << "Error";
    } else {
        qDebug() << "Success";
    }

 /*   //  =====IT WAS JUST FOR CREATE TABLES=====
    QString query1 = "CREATE TABLE users ("
             "Login VarChar(20),"
             "Password VarChar(64),"
             "Salt VarChar(16));";

    QSqlQuery qry1;
    if (!qry1.exec(query1)){
        qDebug() << "query crash1";
    } else {
        qDebug() << "query succeed1";
    }



    QString query2 = "CREATE TABLE record ("
            "Nickname VarChar(20),"
            "Points Integer);";
    QSqlQuery qry2;
    if (!qry2.exec(query2)){
        qDebug() << "query crash1";
    } else {
        qDebug() << "query succeed1";
    } */

    QSqlQueryModel *modal = new QSqlQueryModel();
    QSqlQuery *qry = new QSqlQuery();
    qry->prepare("Select * FROM record ORDER BY Points DESC");
    qry->exec();
    modal->setQuery(*qry);
    ui->tableView->setModel(modal);


    QPixmap p(":/Data/Data/rama.png");
    p = p.scaled(300,300);
    ui->forFone->setPixmap(p);

}

MainWindow::~MainWindow() {
    delete scene;
    delete ui;
}

void MainWindow::soundClick(){
    QMediaPlayer *sound = new QMediaPlayer();
    sound->setMedia(QUrl("C:/MERGE/Agar.IQ/client/Data/zvuk11.mp3"));
    sound->play();
}

void MainWindow::on_startButton_clicked()
{
    scene->show();
    soundClick();
    this->close();
    emit signalStartGame();
    emit signalSendColor(color);
    emit signalSendName(username);
}


void MainWindow::on_settingsButton_clicked()
{
    SettingsWindow *sw = new SettingsWindow();
    sw->show();
    sw->setGeometry(680, 280, 800, 500);
    soundClick();
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
    soundClick();
}

void MainWindow::slotNameLog(QString _name){
    username = _name;
    ui->label->setText("Добро пожаловать, " + username);
}

void MainWindow::slotMaxScore(int _score){
    QSqlQuery qry;
    bool flag = true;
    bool go = false;
    if (qry.exec("SELECT Nickname, Points from record WHERE Nickname='"+username+"' ")){
        if (qry.next()){

            if (_score > qry.value(1).toInt()){
                qry.prepare("DELETE from record WHERE Nickname='"+username+"'");
                if (!qry.exec()){
                    qDebug() << "error del";
                } else {
                    qDebug() << "succeed del";
                }
            /*    qry.prepare("INSERT INTO record ("
                              "Nickname,"
                              "Points)"
                              "VALUES (?,?);");
                  qry.addBindValue(username);
                  qry.addBindValue(_score); */
            } else {
                flag = false;
            }
        } else {
            go = true;
        }
    }

    QSqlQuery qury;
    if (flag || go){

        qury.prepare("INSERT INTO record ("
                     "Nickname,"
                     "Points)"
                     "VALUES (?,?);");
        qury.addBindValue(username);
        qury.addBindValue(_score);
    }

    if (!qury.exec()){
        qDebug() << "error add";
    } else {
        qDebug() << "succeed adding" << _score;
    }

}

void MainWindow::on_switchUser_clicked()
{
    soundClick();
    Login *r;
    r = new Login;
    this->close();
    QSettings set("C:/ProjectDB/Agar.IQ/settings", QSettings::IniFormat);
    set.setValue("type", false);
   // registration = new Login;
    r->showFullScreen();

}


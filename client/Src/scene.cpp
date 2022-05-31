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

//    socket->connectToHost("srv13.yeputons.net", 8418);
//    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    socket->connectToHost("127.0.0.1", 80);

    qDebug() << "start";
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

void Scene::slotResultReady() {
    sendToServer();
}

bool Scene::in_bounds(Entity dot) {
    if (abs(worker->players_data[clientID].get_x_position() - dot.get_x_position()) > 950) {
        return false;
    }

    if (abs(worker->players_data[clientID].get_y_position() - dot.get_y_position()) > 550) {
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

    if (!worker->players_data.empty()) {
        fnt.setPixelSize(1);
        painter.setFont(fnt);
        painter.setPen(QPen(Qt::gray,1,Qt::SolidLine,Qt::FlatCap));

        for (int x = -1500; x <= 3500; x += 50) {
            if (abs(worker->players_data[clientID].get_x_position() - x) <= 950) {
                double new_x = center_x + x - worker->players_data[clientID].get_x_position();
                painter.drawLine(new_x, 0, new_x, 1200);
            }
        }

        for (int y = -900; y <= 2000; y += 50) {
            if (abs(worker->players_data[clientID].get_y_position() - y) <= 550) {
                double new_y = center_y - y + worker->players_data[clientID].get_y_position();
                painter.drawLine(0, new_y, 1900, new_y);
            }
        }

        fnt.setPixelSize(35);
        painter.setFont(fnt);
        painter.setPen(QPen(Qt::black,1,Qt::SolidLine,Qt::FlatCap));
    }

    for (auto it : worker->answers_data) {
        if (in_bounds(it)) {

            painter.setBrush(QBrush(it.color, Qt::SolidPattern));
            double new_x = center_x + it.get_x_position() - worker->players_data[clientID].get_x_position();
            double new_y = center_y - it.get_y_position() + worker->players_data[clientID].get_y_position();

            painter.setPen(QPen(Qt::red, 1, Qt::SolidLine, Qt::FlatCap));
            painter.drawEllipse(QPointF(new_x, new_y), 2*it.get_radius(), 2*it.get_radius());

            const QRect rectangle = QRect(new_x - 50 * it.get_radius(), new_y - 50 * it.get_radius(), 100 * it.get_radius(), 100 * it.get_radius());
            QRect boundingRect;

            painter.setPen(QPen(Qt::black,1,Qt::SolidLine,Qt::FlatCap));
            painter.drawText(rectangle, Qt::AlignCenter, QString::number(it.get_number()), &boundingRect);
        }
    }

    for (auto it : worker->food_data) {
        if (in_bounds(it)) {
            painter.setPen(QPen(it.color, 1, Qt::SolidLine, Qt::FlatCap));
            painter.setBrush(QBrush(it.color, Qt::SolidPattern));

            double new_x = center_x + it.get_x_position() - worker->players_data[clientID].get_x_position();
            double new_y = center_y - it.get_y_position() + worker->players_data[clientID].get_y_position();

            painter.drawEllipse(QPointF(new_x, new_y), 2*it.get_radius(), 2*it.get_radius());
        }
    }

    for (int i = 0; i < worker->players_data.size(); i++) {
        if (worker->players_data[i].is_eaten) {
            continue;
        }

        if (i == clientID) {
            painter.setBrush(QBrush(worker->players_data[i].color, Qt::SolidPattern));
            painter.setPen(QPen(worker->players_data[i].color, 1, Qt::SolidLine, Qt::FlatCap));

            painter.drawEllipse(QPointF(center_x, center_y), 2*worker->players_data[i].get_radius(), 2*worker->players_data[i].get_radius());

            fnt.setPixelSize(35);
            painter.setFont(fnt);
            painter.setPen(QPen(Qt::black,1,Qt::SolidLine,Qt::FlatCap));

            const QRect rectangle = QRect(center_x - 500, center_y - 500, 1000, 1000);
            QRect boundingRect;
            painter.drawText(rectangle, Qt::AlignCenter,
                             QString::fromStdString(worker->players_data[i].get_name()), &boundingRect);
        } else if (in_bounds(worker->players_data[i])) {
                double new_x = center_x + worker->players_data[i].get_x_position() - worker->players_data[clientID].get_x_position();
                double new_y = center_y - worker->players_data[i].get_y_position() + worker->players_data[clientID].get_y_position();

                painter.setBrush(QBrush(worker->players_data[i].color, Qt::SolidPattern));
                painter.setPen(QPen(worker->players_data[i].color, 1, Qt::SolidLine, Qt::FlatCap));

                painter.drawEllipse(QPointF(new_x, new_y), 2*worker->players_data[i].get_radius(), 2*worker->players_data[i].get_radius());

                fnt.setPixelSize(35);
                painter.setFont(fnt);
                painter.setPen(QPen(Qt::black,1,Qt::SolidLine,Qt::FlatCap));

                const QRect rectangle = QRect(new_x - 500, new_y - 500, 1000, 1000);
                QRect boundingRect;
                painter.drawText(rectangle, Qt::AlignCenter, QString::fromStdString(worker->players_data[i].get_name()), &boundingRect);
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
//    workerThread.wait();
    emit sendDisconnection();

    isMenu = true;
}

void Scene::on_pushButton_clicked()
{
    emit slotGameFinish();
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

    toServer["blue"] = (int) (worker->player.color.blueF() * 255.0);
    toServer["green"] = (int) (worker->player.color.greenF() * 255.0);
    toServer["red"] = (int) (worker->player.color.redF() * 255.0);

    toServer["bits"] = worker->bits;
    toServer["operandsCount"] = worker->operandsCount;
    toServer["operands"] = worker->operands;

    //    qDebug() << QString::fromStdString(toServer.dump());

    socket->write(QString::fromStdString(toServer.dump()).toLatin1() + '$');
//    socket->waitForBytesWritten(20000);
}

void Scene::sendDisconnection() {
    json toServer;

    // every message from client to server is player data and settings:
    //      { "status":"connected", "name":player_name,
    //       "id":clientID, "angle":player_angle, "bits":bits,
    //       "operandsCount":operandsCount, "operands" }

    toServer["status"] = "disconnected";
    toServer["id"] = clientID;

    socket->write(QString::fromStdString(toServer.dump()).toLatin1() + '$');
}

void Scene::readFromServer()
{
    socket = (QTcpSocket*)sender();

    json fromServer;

    QByteArray array;

    while(!array.contains('$')) {
        array += socket->readAll();
    }

    int bytes = array.indexOf('$') + 1;
    QByteArray message = array.left(bytes);
    array = array.mid(bytes);

    std::string ans = message.toStdString();
    ans.pop_back();
    fromServer = json::parse(ans);

    // first message from server is reply to connection and
    //       client's id (his number in players_data):
    //       {"status":"connected", "initialization":"yes", "id":clientID}

    // every next message is all scene data, include players, answers
    //       and dots datas, and example:
    //       {"status":"connected", "players":[ players data ],
    //        "answers":[ answers data ], "food":[ food data ],
    //        "expr":example }

    if (fromServer["status"] == "eaten") {
        emit on_pushButton_clicked();
    }

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

            int blue_color = player["blue"];
            int green_color = player["green"];
            int red_color = player["red"];
            bool is_eaten = player["status"];

            QString is_correct = QString::fromStdString(player["is_correct"]);

            worker->players_data.push_back({name, x, y, rad, score, is_correct, is_eaten});

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
                int red = food["red"];
                int green = food["green"];
                int blue = food["blue"];

                worker->food_data.push_back({x, y, red, green, blue});
            }
        } else {
            for (auto food : fromServer["food"]) {
                int food_iter = food["iter"];
                double x = food["x"];
                double y = food["y"];
                int red = food["red"];
                int green = food["green"];
                int blue = food["blue"];

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

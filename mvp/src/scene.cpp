#include "scene.h"
#include "ui_scene.h"
#include "settingswindow.h"

Scene::Scene(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Scene)
{
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
        painter.drawText(QPoint(it.get_x_position() - it.get_radius()/4,it.get_y_position() + it.get_radius()/4), QString::number(it.get_number()));     }
    for (auto it : worker->food) {
        painter.setBrush(QBrush(it.color, Qt::SolidPattern));
        painter.drawEllipse(QPointF(it.get_x_position(), it.get_y_position()), 2*it.get_radius(), 2*it.get_radius());
    }

    painter.setBrush(QBrush(worker->player.color, Qt::SolidPattern));
    painter.drawEllipse(QPointF(worker->player.get_x_position(), worker->player.get_y_position()), 2*worker->player.get_radius(), 2*worker->player.get_radius());

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
            worker->player.speed_Y = -5;
        } else if (key->key() == Qt::Key_S){
            worker->player.speed_Y = 5;
        } else if (key->key() == Qt::Key_A){
            worker->player.speed_X = -5;
        } else if (key->key() == Qt::Key_D){
            worker->player.speed_X = 5;
        }
    }
}

void Scene::keyReleaseEvent(QKeyEvent *event){
    if (!isMenu && event->type() == QEvent::KeyRelease){
        QKeyEvent *key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_W){
            worker->player.speed_Y = 0;
        } else if (key->key() == Qt::Key_S){
            worker->player.speed_Y = 0;
        } else if (key->key() == Qt::Key_A){
            worker->player.speed_X = 0;
        } else if (key->key() == Qt::Key_D){
            worker->player.speed_X = 0;
        }
    }
}

void Scene::on_startGameButton_clicked()
{
    ui->startGameButton->hide();
    ui->settingsButton->hide();
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

void Scene::on_settingsButton_clicked() {
    SettingsWindow *sw = new SettingsWindow();
    sw->show();
}

void Scene::slotGameFinish(){
    qDebug() << "finish";
    workerThread.quit();
    workerThread.wait();
 //   delete worker;
    ui->startGameButton->show();
    ui->settingsButton->show();
    isMenu = true;
}

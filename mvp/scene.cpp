#include "scene.h"
#include "ui_scene.h"

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
    for (auto it : worker->entities){
        painter.setBrush(QBrush(it.color, Qt::SolidPattern));
        painter.drawEllipse(QPointF(it.x, it.y), it.r, it.r);
        painter.drawText(QPoint(it.x - it.r/4,it.y + it.r/4), QString::number(it.num));   // добавил
    }
    fnt.setPixelSize(40);
    painter.setFont(fnt);
    painter.drawText(QPoint(1700,40), worker->text);
    painter.drawText(QPoint(1820,40), QString::number(worker->score));
    painter.drawText(QPoint(800,40), QString::fromLocal8Bit(worker->expr.c_str()));

}

void Scene::keyPressEvent(QKeyEvent *event){
    if (!isMenu && event->type() == QEvent::KeyPress){
        QKeyEvent *key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_W){
            worker->entities[0].speedY = -5;
        } else if (key->key() == Qt::Key_S){
            worker->entities[0].speedY = 5;
        } else if (key->key() == Qt::Key_A){
            worker->entities[0].speedX = -5;
        } else if (key->key() == Qt::Key_D){
            worker->entities[0].speedX = 5;
        }
    }
}

void Scene::keyReleaseEvent(QKeyEvent *event){
    if (!isMenu && event->type() == QEvent::KeyRelease){
        QKeyEvent *key = static_cast<QKeyEvent*>(event);
        if (key->key() == Qt::Key_W){
            worker->entities[0].speedY = 0;
        } else if (key->key() == Qt::Key_S){
            worker->entities[0].speedY = 0;
        } else if (key->key() == Qt::Key_A){
            worker->entities[0].speedX = 0;
        } else if (key->key() == Qt::Key_D){
            worker->entities[0].speedX = 0;
        }
    }
}

void Scene::on_startGameButton_clicked()
{
    ui->startGameButton->hide();
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
 //   delete worker;
    ui->startGameButton->show();
    isMenu = true;
}

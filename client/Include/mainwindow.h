#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scene.h"
#include <QtSql>
#include <QSqlTableModel>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

signals:
    void signalStartGame();
    void signalSendColor(QColor _color);
    void signalSendName(QString _name);

public slots:
    void slotNameLog(QString _name);
    void slotMaxScore(int _score);

private slots:
    void on_startButton_clicked();

    void on_settingsButton_clicked();

    void on_greenButton_clicked();

    void on_yellowButton_clicked();

    void on_purpleButton_clicked();

    void on_closeButton_clicked();

    void on_switchUser_clicked();

private:
    Ui::MainWindow *ui;
    Scene *scene = nullptr;

    QSqlDatabase db;
    QSqlTableModel *model;
    QString username;
    QColor color = Qt::white;

    void soundClick();


protected:
    void paintEvent(QPaintEvent *event);

};
#endif // MAINWINDOW_H

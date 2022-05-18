#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMessageBox>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include "mainwindow.h"

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    QString generateSalt();
    ~Login();

private slots:
    void on_logButton_clicked();

    void on_regButton_clicked();

    void on_guestButton_clicked();

signals:
    void signalNameLog(QString _name);

private:
    Ui::Login *ui;
    MainWindow *w;
    QString hash;
    QString salt;
};



#endif // LOGIN_H
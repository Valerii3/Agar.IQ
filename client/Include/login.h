#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QMessageBox>
#include <QRandomGenerator>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <sstream>
#include <iomanip>
#include <string>
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
   // QString sha256(const std::string str);
  //  QString sha256(const std::string str);
    ~Login();

private slots:
    void on_logButton_clicked();

    void on_regButton_clicked();

    void on_guestButton_clicked();

    void on_aut_clicked();



signals:
    void signalNameLog(QString _name);

private:
    Ui::Login *ui;
    MainWindow *w;
    QString hash;
    QString salt;
    bool autoLog = false;
};



#endif // LOGIN_H

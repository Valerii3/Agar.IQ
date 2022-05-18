#include "../Include/login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    w = new MainWindow;
    connect(this, &Login::signalNameLog, w, &MainWindow::slotNameLog);
}

Login::~Login()
{
    delete ui;
}

void Login::on_logButton_clicked()
{
    QString login = ui->logName->text();
    QString pswd = ui->logPswd->text();
    QCryptographicHash calcl256(QCryptographicHash::Sha256);
    calcl256.addData(pswd.toLatin1());
    hash = calcl256.result().toHex().data();
    QSqlQuery qry;
    if (qry.exec("SELECT Login, Password from users WHERE Login=\'" + login +
                 "\' AND Password=\'" + hash + "\'")){
        if (qry.next()){
            qDebug() << "user exists";
            emit signalNameLog(login);
            this->hide();
            w->show();
        } else {
            QMessageBox::about(this, "error", "user doesnt exists");
        }
    }

}


void Login::on_regButton_clicked()
{
    QString name = ui->regName->text();
    QString pswd = ui->regPswd1->text();
    QString _pswd = ui->regPswd2->text();
    if (pswd == _pswd){
        QSqlQuery qry;
        QCryptographicHash calcl256(QCryptographicHash::Sha256);
        calcl256.addData(pswd.toLatin1());
        hash = calcl256.result().toHex().data();
        qry.prepare("INSERT INTO users ("
                    "Login,"
                    "Password)"
                    "VALUES (?,?);");
        qry.addBindValue(name);
        qry.addBindValue(hash);


        if (!qry.exec()){
            qDebug() << "error add";
        } else {
            qDebug() << "succeed adding";
            QMessageBox::about(this, "Успешная регистрация", "Пользователь успешно добавлен");
        }
    } else {
        QMessageBox::about(this, "Adding error", "Пароли не совпадают");
    }
}


void Login::on_guestButton_clicked()
{
    emit signalNameLog("Гость");
    w->show();
    this->close();
}


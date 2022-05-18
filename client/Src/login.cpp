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
    QSqlQuery qry;
    if (qry.exec("SELECT Login, Password from users WHERE Login=\'" + login +
                 "\' AND Password=\'" + pswd + "\'")){
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

        qry.prepare("INSERT INTO users ("
                    "Login,"
                    "Password)"
                    "VALUES (?,?);");
        qry.addBindValue(name);
        qry.addBindValue(pswd);


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


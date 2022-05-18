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
    ;
    QSqlQuery qry;
    if (qry.exec("SELECT Login, Password, Salt from users WHERE Login=\'" + login +
                 "\' ")){
        if (qry.next()){
            QString hashDB = qry.value(1).toString();
            salt = qry.value(2).toString();

            QCryptographicHash calcl256(QCryptographicHash::Sha256);
            calcl256.addData(salt.toLatin1() + pswd.toLatin1());
            hash = calcl256.result().toHex().data();
            if (hash == hashDB){
                qDebug() << "user exists";
                emit signalNameLog(login);
                this->hide();
                w->show();
            }

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
        salt = generateSalt();
        calcl256.addData(salt.toLatin1() + pswd.toLatin1());
        hash = calcl256.result().toHex().data();
        qry.prepare("INSERT INTO users ("
                    "Login,"
                    "Password,"
                    "Salt)"
                    "VALUES (?,?,?);");
        qry.addBindValue(name);
        qry.addBindValue(hash);
        qry.addBindValue(salt);

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

QString Login::generateSalt(){
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 16;
    QString randomString;

    for(int i=0; i<randomStringLength; ++i){
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

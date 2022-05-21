#include "../Include/login.h"
#include "ui_login.h"

QString sha256(const std::string str)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::stringstream ss;
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return QString::fromStdString(ss.str());
}


Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    w = new MainWindow;
    connect(this, &Login::signalNameLog, w, &MainWindow::slotNameLog);
    QSettings set("C:/ProjectDB/Agar.IQ/settings", QSettings::IniFormat);   // свой путь
    bool type = set.value("type").toBool();
    if (type){
        QString lgn = set.value("login").toString();
        emit signalNameLog(lgn);
        this->hide();
        w->show();

    }
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
    if (qry.exec("SELECT Login, Password, Salt from users WHERE Login=\'" + login +
                 "\' ")){
        if (qry.next()){
            QString hashDB = qry.value(1).toString();
            salt = qry.value(2).toString();
            hash = sha256(salt.toStdString() + pswd.toStdString());
            QSettings set("C:/ProjectDB/Agar.IQ/settings", QSettings::IniFormat);     // свой путь
            if (hash == hashDB){
                if (autoLog){
                    set.setValue("type", true);
                    set.setValue("login", login);
                    set.setValue("password", hash);
                    qDebug() << "user exists";
                    emit signalNameLog(login);
                    this->hide();
                    w->show();
                } else {
                    set.setValue("type", false);
                    qDebug() << "user exists";
                    emit signalNameLog(login);
                    this->hide();
                    w->show();
                }

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
        salt = generateSalt();
        hash = sha256(salt.toStdString() + pswd.toStdString());
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






void Login::on_autoLogin_clicked()
{
    autoLog = true;
}


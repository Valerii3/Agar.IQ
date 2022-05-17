#include "../Include/mainwindow.h"
#include "../Include/login.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    Login l;
    l.show();
 //   MainWindow w;
 //   w.show();
    return a.exec();
}

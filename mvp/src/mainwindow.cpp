#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    //  scene = new Scene;
    // ui->scene->setParent(scene);
    //   scene->show();
    //   this->hide();
}

MainWindow::~MainWindow() {
    delete scene;
    delete ui;
}


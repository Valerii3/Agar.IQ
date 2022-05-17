#include "../Include/settingswindow.h"
#include "ui_settingswindow.h"
#include "../Include/question.h"
#include "../Include/worker.h"
#include <QLabel>

SettingsWindow::SettingsWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::SettingsWindow) {
    ui->setupUi(this);
}

SettingsWindow::~SettingsWindow() {
    delete ui;
}

void SettingsWindow::on_saveSettingsButton_clicked() {
    std::string operands;
    if (ui->addCheckBox->isChecked()) {
        operands += '+';
    }
    if (ui->subCheckBox->isChecked()) {
        operands += '-';
    }
    if (ui->mulCheckBox->isChecked()) {
        operands += '*';
    }
    Worker::bits = ui->bitsBox->value();
    Worker::operandsCount = ui->operandsCountBox->value();
    Worker::operands = operands;
    hide();
}


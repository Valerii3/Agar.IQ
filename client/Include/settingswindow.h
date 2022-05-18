#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class SettingsWindow; }
QT_END_NAMESPACE

class SettingsWindow : public QMainWindow {
    Q_OBJECT

public:
    SettingsWindow(QWidget *parent = nullptr);

    ~SettingsWindow();

    signals:

private slots:
     void on_saveSettingsButton_clicked();

private:
    Ui::SettingsWindow *ui;

};

#endif // SETTINGSWINDOW_H

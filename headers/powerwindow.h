#ifndef POWERWINDOW_H
#define POWERWINDOW_H

#include <QTimer>
#include <QMovie>
#include <windows.h>
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class powerwindow; }
QT_END_NAMESPACE

class powerwindow : public QWidget {
    Q_OBJECT

public:
    explicit powerwindow(QWidget *parent = nullptr);
    ~powerwindow() override;

private slots:
    void updatePowerInfo();
    void on_sleepButton_clicked();
    void on_hibernateButton_clicked();
    void on_backButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::powerwindow *ui;
    QTimer *timer;
    QMovie *purpleMovie;
    mainwindow *mainWindow;
    void updateAnimation(bool isCharging);
    QString getPowerType();
    QString getBatteryType();
    QString getBatteryTypeFallback();
    int getBatteryLevel();
    QString getPowerSaverMode();
    QString getBatteryLifeTime();
    QString getRemainingTime();
};

#endif //POWERWINDOW_H
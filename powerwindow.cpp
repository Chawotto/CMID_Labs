#include "headers/powerwindow.h"
#include "ui_powerwindow.h"
#include <powrprof.h>
#include <QDir>
#include <QFile>
#include <QApplication>
#include <QDebug>
#include <QProcess>


powerwindow::powerwindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::powerwindow), mainWindow(nullptr) {
    ui->setupUi(this);
    purpleMovie = new QMovie("C:/users/alexe/Desktop/ilabs/images/happy.png");
    if (!purpleMovie->isValid()) {
        qDebug() << "Failed to load purple_menu.gif";
    }
    ui->purpleLabel->setMovie(purpleMovie);
    purpleMovie->start();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &powerwindow::updatePowerInfo);
    timer->start(1000);
    updatePowerInfo();
}

powerwindow::~powerwindow() {
    delete purpleMovie;
    delete timer;
    delete ui;
}

void powerwindow::updatePowerInfo() {
    SYSTEM_POWER_STATUS status;
    GetSystemPowerStatus(&status);

    ui->powerTypeLabel->setText("Тип энергопитания: " + getPowerType());
    ui->batteryTypeLabel->setText("Тип батареи: " + getBatteryType());
    int batteryLevel = getBatteryLevel();
    ui->batteryLevelLabel->setText(batteryLevel >= 0 ? QString("Уровень заряда: %1%").arg(batteryLevel) : "Уровень заряда: Неизвестно");
    ui->powerModeLabel->setText("Режим энергосбережения: " + getPowerSaverMode());
    ui->batteryLifeLabel->setText("Время работы аккумулятора: " + getBatteryLifeTime());
    ui->remainingTimeLabel->setText("Оставшееся время: " + getRemainingTime());
    updateAnimation(status.ACLineStatus == 1);
}

void powerwindow::updateAnimation(bool isCharging) {
    QString gifPath = isCharging ? "C:/users/alexe/Desktop/ilabs/images/wow.png" : "C:/users/alexe/Desktop/ilabs/images/wow3.png";
    if (purpleMovie->fileName() != gifPath) {
        purpleMovie->stop();
        delete purpleMovie;
        purpleMovie = new QMovie(gifPath);
        if (!purpleMovie->isValid()) {
            qDebug() << "Failed to load GIF:" << gifPath;
        }
        ui->purpleLabel->setMovie(purpleMovie);
        purpleMovie->start();
    }
}

QString powerwindow::getPowerType() {
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        if (status.ACLineStatus == 1) return "От сети";
        if (status.ACLineStatus == 0) return "От батареи";
        if (status.ACLineStatus == 255) return "Статус неизвестен";
    }
    return "Неизвестно";
}

QString powerwindow::getBatteryType() {
    QProcess process;
    QString reportPath = QDir::tempPath() + "/battery_report.html";
    process.start("powercfg", QStringList() << "/batteryreport" << "/output" << reportPath);
    process.waitForFinished(3000);

    QFile reportFile(reportPath);
    if (!reportFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return getBatteryTypeFallback();
    }

    QString reportContent = reportFile.readAll();
    reportFile.close();
    QFile::remove(reportPath);

    QRegularExpression chemistryRegex("Chemistry.*?<td[^>]*>(.*?)</td>",
                                      QRegularExpression::CaseInsensitiveOption |
                                      QRegularExpression::DotMatchesEverythingOption);

    QRegularExpressionMatch match = chemistryRegex.match(reportContent);
    if (match.hasMatch()) {
        QString chemistry = match.captured(1).trimmed();
        chemistry.remove(QRegularExpression("<[^>]*>"));
        if (!chemistry.isEmpty()) {
            return chemistry;
        }
    }

    return getBatteryTypeFallback();
}

QString powerwindow::getBatteryTypeFallback() {
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        if (status.BatteryFlag == 128) return "Батарея отсутствует";
        return "Батарея присутствует";
    }
    return "Информация недоступна";
}

int powerwindow::getBatteryLevel() {
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        if (status.BatteryLifePercent <= 100) {
            return status.BatteryLifePercent;
        }
    }
    return -1;
}

QString powerwindow::getPowerSaverMode() {
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        return (status.SystemStatusFlag == 1) ? "Включен" : "Выключен";
    }
    return "Неизвестно";
}

QString powerwindow::getBatteryLifeTime() {
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        if (status.ACLineStatus == 1) {
            return "Подключено к сети";
        }
        if (status.BatteryLifeTime == (DWORD)-1) {
            return "Расчет времени...";
        }

        int seconds = status.BatteryLifeTime;
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        return QString("%1 ч %2 мин").arg(hours).arg(minutes);
    }
    return "Неизвестно";
}

QString powerwindow::getRemainingTime() {
    SYSTEM_POWER_STATUS status;
    if (GetSystemPowerStatus(&status)) {
        if (status.ACLineStatus == 1) {
            return "Зарядное устройство подключено";
        }
        if (status.BatteryLifeTime == (DWORD)-1) {
            return "Расчет времени...";
        }

        int seconds = status.BatteryLifeTime;
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        return QString("≈%1 ч %2 мин").arg(hours).arg(minutes);
    }
    return "Неизвестно";
}

void powerwindow::on_sleepButton_clicked() {
    SetSuspendState(FALSE, FALSE, FALSE);
}

void powerwindow::on_hibernateButton_clicked() {
    SetSuspendState(TRUE, FALSE, FALSE);
}

void powerwindow::on_backButton_clicked() {
    if (mainwindow::currentWindow) {
        mainwindow::currentWindow->close();
    }
    mainWindow = new mainwindow();
    mainWindow->setWindowFlags(Qt::Window);
    mainWindow->show();
    mainwindow::currentWindow = mainWindow;
    this->close();
}

void powerwindow::on_exitButton_clicked() {
    if (mainwindow::currentWindow) {
        mainwindow::currentWindow->close();
    }
    QApplication::quit();
}
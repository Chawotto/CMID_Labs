#include "headers/devwindow.h"
#include "ui_devwindow.h"
#include <QApplication>
#include <QDebug>
#include "headers/mainwindow.h"

devwindow::devwindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::devwindow), mainWindow(nullptr) {
    ui->setupUi(this);
    QPixmap pixmap("C:/users/alexe/Desktop/ilabs/images/sad.png");
    if (pixmap.isNull()) {
        qDebug() << "Failed to load purple_sad.png";
    }
    ui->sadPurpleLabel->setPixmap(pixmap);
}

devwindow::~devwindow() {
    delete ui;
}

void devwindow::on_backButton_clicked() {
    if (mainwindow::currentWindow) {
        mainwindow::currentWindow->close();
    }
    mainWindow = new mainwindow();
    mainWindow->setWindowFlags(Qt::Window);
    mainWindow->show();
    mainwindow::currentWindow = mainWindow;
    this->close();
}

void devwindow::on_exitButton_clicked() {
    if (mainwindow::currentWindow) {
        mainwindow::currentWindow->close();
    }
    QApplication::quit();
}
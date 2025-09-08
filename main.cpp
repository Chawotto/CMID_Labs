#include "headers/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    mainwindow w;
    mainwindow::currentWindow = &w;
    w.setWindowFlags(Qt::Window);
    w.show();
    return a.exec();
}
#ifndef DEVWINDOW_H
#define DEVWINDOW_H
#include "mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class devwindow; }
QT_END_NAMESPACE

class devwindow : public QWidget {
    Q_OBJECT

public:
    explicit devwindow(QWidget *parent = nullptr);
    ~devwindow() override;

private slots:
    void on_backButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::devwindow *ui;
    mainwindow *mainWindow;
};

#endif //DEVWINDOW_H
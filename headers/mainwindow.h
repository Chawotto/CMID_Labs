#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QList>
#include <QPixmap>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class mainwindow; }
QT_END_NAMESPACE

class mainwindow : public QWidget {
    Q_OBJECT

public:
    explicit mainwindow(QWidget *parent = nullptr);
    ~mainwindow() override;
    static QWidget *currentWindow;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private slots:
    void on_lab1Button_clicked();
    void on_lab2Button_clicked();
    void on_lab3Button_clicked();
    void on_lab4Button_clicked();
    void on_lab5Button_clicked();
    void on_lab6Button_clicked();
    void on_exitButton_clicked();
    void updateAnimationFrame();

private:
    Ui::mainwindow *ui;
    QList<QPixmap> preloadedMainFrames;
    QTimer *animationTimer;
    int currentFrame;
    void setDefaultImage();
    void setDescriptionText(const QString &text);
};

#endif //MAINWINDOW_H
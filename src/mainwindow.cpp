#include "headers/mainwindow.h"
#include "ui_mainwindow.h"
#include "headers/powerwindow.h"
#include "headers/devwindow.h"
#include <QApplication>
#include <QDebug>
#include <QEvent>

QWidget *mainwindow::currentWindow = nullptr;

mainwindow::mainwindow(QWidget *parent) :
    QWidget(parent), ui(new Ui::mainwindow), currentFrame(0) {
    ui->setupUi(this);

    ui->lab1Button->installEventFilter(this);
    ui->lab2Button->installEventFilter(this);
    ui->lab3Button->installEventFilter(this);
    ui->lab4Button->installEventFilter(this);
    ui->lab5Button->installEventFilter(this);
    ui->lab6Button->installEventFilter(this);
    ui->exitButton->installEventFilter(this);

    for (int i = 0; i <= 39; ++i) {
        QString framePath = QString("C:/Users/alexe/Desktop/ilabs/images/frames/main_%1.png").arg(i, 3, 10, QChar('0'));
        QPixmap frame(framePath);
        if (!frame.isNull()) {
            preloadedMainFrames.append(frame.scaled(ui->purpleLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            qDebug() << "err" << framePath;
        }
    }

    qDebug() << "Loaded frames:" << preloadedMainFrames.count();

    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &mainwindow::updateAnimationFrame);

    if (preloadedMainFrames.isEmpty()) {
        qDebug() << "err";
        ui->purpleLabel->setText("err");
    } else {
        setDefaultImage();
        qDebug() << "done";
    }

    setDescriptionText("Данная программа написана на C++ и реализует лабораторный практикум по «Интерфейсам и устройствам вычислительных машин»."
                      "\nВключает шесть лабораторных работ, запускаемых через интерфейс с уникальными персонажами мультфильма Purple and Brown.");
}

mainwindow::~mainwindow() {
    delete animationTimer;
    delete ui;
}

void mainwindow::setDefaultImage() {
    if (preloadedMainFrames.isEmpty()) {
        qDebug() << "err";
        ui->purpleLabel->setText("err");
    } else {
        currentFrame = 0;
        ui->purpleLabel->setPixmap(preloadedMainFrames[currentFrame]);
        animationTimer->start(100); // 100 мс (~10 кадров/с)
        qDebug() << "done";
    }
}

void mainwindow::updateAnimationFrame() {
    currentFrame = (currentFrame + 1) % preloadedMainFrames.count();
    ui->purpleLabel->setPixmap(preloadedMainFrames[currentFrame]);
}

void mainwindow::setDescriptionText(const QString &text) {
    ui->descriptionLabel->setText(text);
}

bool mainwindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::Enter) {
        QString imagePath;
        QString description;
        if (obj == ui->lab1Button) {
            imagePath = "C:/Users/alexe/Desktop/ilabs/images/lab1.png";
            description = "Лабораторная работа 1. \nЭнергопитание:\nВывод информации об энергопитании компьютера: тип питания, тип батареи, уровень заряда, режим энергосбережения. Реализация перехода в спящий режим и гибернацию из консоли. Вывод времени работы аккумулятора без зарядки и оставшегося времени работы.";

        } else if (obj == ui->lab2Button) {
            imagePath = "C:/Users/alexe/Desktop/ilabs/images/lab2.png";
            description = "Лабораторная работа 2. \nКонфигурационное пространство PCI:\nВывод списка устройств, подключенных к шине PCI, с их характеристиками (DeviceID, VendorID) в виде таблицы. Подключение к шине через порты ввода-вывода без использования готовых библиотек.";
        } else if (obj == ui->lab3Button) {
            imagePath = "C:/Users/alexe/Desktop/ilabs/images/lab3.png";
            description = "Лабораторная работа 3. \nУстройства хранения данных:\nВывод данных о диске (HDD или SSD): модель, изготовитель, серийный номер, версия прошивки, объем памяти, тип интерфейса, поддерживаемые режимы. Подключение через порты ввода-вывода без использования готовых библиотек.";
        } else if (obj == ui->lab4Button) {
            imagePath = "C:/Users/alexe/Desktop/ilabs/images/lab4.png";
            description = "Лабораторная работа 4. \nВеб-камера:\nВывод информации о веб-камере. Захват фото и видео с сохранением в файл. Реализация скрытого режима фото- или видеонаблюдения без отображения приложения на мониторе и панели задач.";
        } else if (obj == ui->lab5Button) {
            imagePath = "C:/Users/alexe/Desktop/ilabs/images/lab5.png";
            description = "Лабораторная работа 5. \nUSB-устройства:\nМониторинг USB-портов: отслеживание подключения и извлечения устройств, события безопасного и небезопасного извлечения, отказ в извлечении. Программное безопасное извлечение USB-устройств (мышь, флешка).";
        } else if (obj == ui->lab6Button) {
            imagePath = "C:/Users/alexe/Desktop/ilabs/images/lab6.png";
            description = "Лабораторная работа 6. \nРабота с интерфейсом Bluetooth:\nМониторинг Bluetooth-устройств, подключение к устройству и передача файла. Варианты: передача на умную колонку с автопроигрыванием, на ноутбук или телефон с автопроигрыванием (с разной сложностью).";
        } else if (obj == ui->exitButton) {
            imagePath = "C:/Users/alexe/Desktop/ilabs/images/sad.png";
            description = "Выход из приложения";
        }

        if (!imagePath.isEmpty()) {
            animationTimer->stop();
            QPixmap pixmap(imagePath);
            if (pixmap.isNull()) {
                qDebug() << "err" << imagePath;
                ui->purpleLabel->setText("err");
            } else {
                ui->purpleLabel->setPixmap(pixmap.scaled(ui->purpleLabel->size(), Qt::KeepAspectRatio));
                qDebug() << imagePath << "done";
            }
            setDescriptionText(description);
        }
        return true;
    } else if (event->type() == QEvent::Leave) {
        setDefaultImage();
        setDescriptionText("Данная программа написана на C++ и реализует лабораторный практикум по «Интерфейсам и устройствам вычислительных машин»."
                          "\nВключает шесть лабораторных работ, запускаемых через интерфейс с уникальным персонажем мультфильма.");
        return true;
    }
    return QWidget::eventFilter(obj, event);
}

void mainwindow::on_lab1Button_clicked() {
    if (currentWindow) {
        currentWindow->close();
    }
    powerwindow *powerWindow = new powerwindow();
    powerWindow->setWindowFlags(Qt::Window);
    powerWindow->show();
    currentWindow = powerWindow;
}

void mainwindow::on_lab2Button_clicked() {
    if (currentWindow) {
        currentWindow->close();
    }
    devwindow *devWindow = new devwindow();
    devWindow->setWindowFlags(Qt::Window);
    devWindow->show();
    currentWindow = devWindow;
}

void mainwindow::on_lab3Button_clicked() {
    if (currentWindow) {
        currentWindow->close();
    }
    devwindow *devWindow = new devwindow();
    devWindow->setWindowFlags(Qt::Window);
    devWindow->show();
    currentWindow = devWindow;
}

void mainwindow::on_lab4Button_clicked() {
    if (currentWindow) {
        currentWindow->close();
    }
    devwindow *devWindow = new devwindow();
    devWindow->setWindowFlags(Qt::Window);
    devWindow->show();
    currentWindow = devWindow;
}

void mainwindow::on_lab5Button_clicked() {
    if (currentWindow) {
        currentWindow->close();
    }
    devwindow *devWindow = new devwindow();
    devWindow->setWindowFlags(Qt::Window);
    devWindow->show();
    currentWindow = devWindow;
}

void mainwindow::on_lab6Button_clicked() {
    if (currentWindow) {
        currentWindow->close();
    }
    devwindow *devWindow = new devwindow();
    devWindow->setWindowFlags(Qt::Window);
    devWindow->show();
    currentWindow = devWindow;
}

void mainwindow::on_exitButton_clicked() {
    if (currentWindow) {
        currentWindow->close();
    }
    QApplication::quit();
}
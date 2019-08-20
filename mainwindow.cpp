#include "mainwindow.h"
#include "gclass.h"
#include "ui_mainwindow.h"

#include <QTime>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    showFullScreen();
    ui->label_3->setText("<FONT COLOR=#008000>ВХОД</FONT>");
    ui->label_4->setText("или");
    ui->label_5->setText("<FONT COLOR=#f00060>ВЫХОД</FONT>");
    //setSegmentStyle(Filled);
    timer = new QTimer;
    //Связываем сигнал переполнения таймера со слотом
    QObject::connect(timer, SIGNAL(timeout()), this, SLOT(timer_overflow()));
    //Задаем время срабатывания таймера (в мс)
    timer->start(100);
}

MainWindow::~MainWindow()
{

    delete ui;
}


void MainWindow::keyPressEvent(QKeyEvent * event)
{
    if (QString::number(event->key()) == "90"){
        QMessageBox msgBox;
        msgBox.setText("z");
       // msgBox.exec();
        f2 = new in;
            this->hide();
            f2->show();
    }
    if (QString::number(event->key()) == "88"){
        QMessageBox msgBox;
        msgBox.setText("z");
       // msgBox.exec();
        f3 = new exi;
            this->hide();
            f3->show();
    }

}

void MainWindow::timer_overflow()
{
    //Обновляем значение времени на форме
    QTime time = QTime::currentTime();
    QString text = time.toString("hh:mm");
    if ((time.second() % 2) == 0)
        text[2] = ' ';
    ui->lcdNumber->display(text);
}



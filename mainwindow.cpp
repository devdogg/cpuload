#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(recount()));
    timer->start(1000);
    double value = this->getCpuLoadValue();
    ui->cpuNumber->display(value);
    ui->progressBar->setValue(value);
    ui->progressBar->setFormat(QString::number(value, 'f', 2)+"%");
}

MainWindow::~MainWindow()
{
    delete ui;
}

double MainWindow::getCpuLoadValue()
{
    QFile file("/proc/stat");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return -1;
    QByteArray arr = file.readLine(255);
    arr.remove(0, 5);
    QList<QByteArray> list = arr.split(' ');

    double totalValue = list.at(0).toDouble() + list.at(1).toDouble() +
            list.at(2).toDouble() + list.at(3).toDouble();
    double cpumode = list.at(2).toDouble();
    double systemValue = (cpumode / totalValue) * 100;
    return systemValue;
}

void MainWindow::recount()
{
    double value = this->getCpuLoadValue();
    ui->cpuNumber->display(value);
    ui->progressBar->setValue(value);
    ui->progressBar->setFormat(QString::number(value, 'f', 2)+"%");
}

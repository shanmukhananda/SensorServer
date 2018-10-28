#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QAccelerometer>
#include <QDebug>
#include <QGyroscope>
#include <chrono>
#include <thread>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    accel = new QAccelerometer(this);
    accel->start();
    gyro = new QGyroscope(this);
    gyro->start();

    print();
}

void MainWindow::print() {
    while (true) {
        auto accel_reading = accel->reading();
        qDebug() << "accel_x:" << accel_reading->x()
                 << "accel_y:" << accel_reading->y()
                 << "accel_z:" << accel_reading->z();

        auto gyro_reading = gyro->reading();
        qDebug()
            << "gyro_x:" << gyro_reading->x() << "gyro_y:" << gyro_reading->y()
            << "gyro_z:" << gyro_reading->z();

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

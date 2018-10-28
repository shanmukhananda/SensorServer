#include "common/pch.h"

#include "ui_mainwindow.h"
#include "view/mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    QColor col = QColor(Qt::green);
    QString qss = QString("background-color: %1").arg(col.name());
    ui->pushButton_start_stop->setStyleSheet(qss);

    auto url = QUrl(QLatin1String("qrc:/camera.qml"));
    ui->quickWidget_camera->setSource(url);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::enabled_controls(bool val) {
    ui->lineEdit_ip->setEnabled(val);
    ui->lineEdit_port->setEnabled(val);

    ui->radioButton_640_x_480->setEnabled(val);
    ui->radioButton_1280_x_720->setEnabled(val);

    ui->checkBox_accelerometer->setEnabled(val);
    ui->checkBox_camera->setEnabled(val);
    ui->checkBox_gps->setEnabled(val);
    ui->checkBox_gyroscope->setEnabled(val);
}

void MainWindow::on_pushButton_start_stop_toggled(bool checked) {
    if (checked) {
        qDebug() << "Started";
        QColor col = QColor(Qt::red);
        QString qss = QString("background-color: %1").arg(col.name());
        ui->pushButton_start_stop->setStyleSheet(qss);
        ui->pushButton_start_stop->setText("Stop");
        enabled_controls(false);
    } else {
        qDebug() << "Stopped";
        QColor col = QColor(Qt::green);
        QString qss = QString("background-color: %1").arg(col.name());
        ui->pushButton_start_stop->setStyleSheet(qss);
        ui->pushButton_start_stop->setText("Start");
        enabled_controls(true);
    }
}

void MainWindow::on_checkBox_accelerometer_stateChanged(int arg1) {
    if (Qt::Checked == arg1)
        qDebug() << "Accelerometer Checked";
    else if (Qt::Unchecked == arg1)
        qDebug() << "Accelerometer Unchecked";
}

void MainWindow::on_checkBox_camera_stateChanged(int arg1) {
    if (Qt::Checked == arg1)
        qDebug() << "Camera Checked";
    else if (Qt::Unchecked == arg1)
        qDebug() << "Camera Unchecked";
}

void MainWindow::on_checkBox_gyroscope_stateChanged(int arg1) {
    if (Qt::Checked == arg1)
        qDebug() << "Gyroscope Checked";
    else if (Qt::Unchecked == arg1)
        qDebug() << "Gyroscope Unchecked";
}

void MainWindow::on_checkBox_gps_stateChanged(int arg1) {
    if (Qt::Checked == arg1)
        qDebug() << "GPS Checked";
    else if (Qt::Unchecked == arg1)
        qDebug() << "GPS Unchecked";
}

void MainWindow::on_radioButton_1280_x_720_toggled(bool checked) {
    if (checked)
        qDebug() << "Resolution changed to 1280 x 720";
}

void MainWindow::on_radioButton_640_x_480_toggled(bool checked) {
    if (checked)
        qDebug() << "Resolution changed to 640 x 480";
}

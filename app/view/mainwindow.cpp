#include "common/pch.h"

#include "ui_mainwindow.h"
#include "view/mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    auto color = QColor(Qt::green);
    auto qss = QString("background-color: %1").arg(color.name());
    ui->pushButton_start_stop->setStyleSheet(qss);

    auto url = QUrl(QLatin1String("qrc:/camera.qml"));
    ui->quickWidget_camera->setSource(url);

    auto root_object = ui->quickWidget_camera->rootObject();

    auto qml_camera = root_object->findChild<QObject*>("camera");
    _camera = qvariant_cast<QCamera*>(qml_camera->property("mediaObject"));
    _camera->stop();

    _camera_settings.setMinimumFrameRate(30.0);
    _camera_settings.setMaximumFrameRate(30.0);
    _camera_settings.setResolution(QSize(640, 480));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::enabled_controls(bool is_enabled) {
    ui->lineEdit_ip->setEnabled(is_enabled);
    ui->lineEdit_port->setEnabled(is_enabled);

    ui->radioButton_640_x_480->setEnabled(is_enabled);
    ui->radioButton_1280_x_720->setEnabled(is_enabled);

    ui->checkBox_accelerometer->setEnabled(is_enabled);
    ui->checkBox_camera->setEnabled(is_enabled);
    ui->checkBox_gps->setEnabled(is_enabled);
    ui->checkBox_gyroscope->setEnabled(is_enabled);
}

void MainWindow::on_pushButton_start_stop_toggled(bool is_checked) {
    if (is_checked) {
        qDebug() << "Started";
        auto color = QColor(Qt::red);
        auto qss = QString("background-color: %1").arg(color.name());
        ui->pushButton_start_stop->setStyleSheet(qss);
        ui->pushButton_start_stop->setText("Stop");
        enabled_controls(false);

        if(resolution::hd == _resolution)
            _camera_settings.setResolution(QSize(1280, 720));
        else if(resolution::vga == _resolution)
            _camera_settings.setResolution(QSize(640, 480));

        _camera->setViewfinderSettings(_camera_settings);
        _camera->start();

    } else {
        qDebug() << "Stopped";
        auto color = QColor(Qt::green);
        auto qss = QString("background-color: %1").arg(color.name());
        ui->pushButton_start_stop->setStyleSheet(qss);
        ui->pushButton_start_stop->setText("Start");
        enabled_controls(true);

        _camera->stop();
    }
}

void MainWindow::on_checkBox_accelerometer_stateChanged(int checkstate) {
    if (Qt::Checked == checkstate)
        qDebug() << "Accelerometer Checked";
    else if (Qt::Unchecked == checkstate)
        qDebug() << "Accelerometer Unchecked";
}

void MainWindow::on_checkBox_camera_stateChanged(int checkstate) {
    if (Qt::Checked == checkstate)
        qDebug() << "Camera Checked";
    else if (Qt::Unchecked == checkstate)
        qDebug() << "Camera Unchecked";
}

void MainWindow::on_checkBox_gyroscope_stateChanged(int checkstate) {
    if (Qt::Checked == checkstate)
        qDebug() << "Gyroscope Checked";
    else if (Qt::Unchecked == checkstate)
        qDebug() << "Gyroscope Unchecked";
}

void MainWindow::on_checkBox_gps_stateChanged(int checkstate) {
    if (Qt::Checked == checkstate)
        qDebug() << "GPS Checked";
    else if (Qt::Unchecked == checkstate)
        qDebug() << "GPS Unchecked";
}

void MainWindow::on_radioButton_1280_x_720_toggled(bool is_checked) {
    if (is_checked) {
        qDebug() << "Resolution changed to 1280 x 720";
       _resolution = resolution::hd;
    }
}

void MainWindow::on_radioButton_640_x_480_toggled(bool is_checked) {
    if (is_checked) {
        qDebug() << "Resolution changed to 640 x 480";
        _resolution = resolution::vga;
    }
}

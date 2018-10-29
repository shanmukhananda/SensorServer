#include "common/pch.h"

#include "presenter/presenter.h"
#include "ui_view.h"
#include "view/view.h"

View::View() : ui(new Ui::View) {
    ui->setupUi(this);
}

View::~View() {
    delete ui;
    ui = nullptr;
}

void View::start() {
    update_start_stop_button(QColor(Qt::green), "Start");
    initialize_camera();
    show();
}

void View::enabled_controls(bool is_enabled) {
    ui->lineEdit_ip->setEnabled(is_enabled);
    ui->lineEdit_port->setEnabled(is_enabled);

    ui->radioButton_640_x_480->setEnabled(is_enabled);
    ui->radioButton_1280_x_720->setEnabled(is_enabled);

    ui->checkBox_accelerometer->setEnabled(is_enabled);
    ui->checkBox_camera->setEnabled(is_enabled);
    ui->checkBox_gps->setEnabled(is_enabled);
    ui->checkBox_gyroscope->setEnabled(is_enabled);
}

void View::update_start_stop_button(QColor color, QString text) {
    auto qss = QString("background-color: %1").arg(color.name());
    ui->pushButton_start_stop->setStyleSheet(qss);
    ui->pushButton_start_stop->setText(text);
}

void View::initialize_camera() {
    auto url = QUrl(QLatin1String("qrc:/camera.qml"));
    ui->quickWidget_camera->setSource(url);

    auto root_object = ui->quickWidget_camera->rootObject();
    auto qml_camera = root_object->findChild<QObject*>("camera");
    auto camera = qvariant_cast<QCamera*>(qml_camera->property("mediaObject"));
    camera->stop();
    emit view_initialized(camera);
}

void View::on_pushButton_start_stop_toggled(bool is_checked) {
    if (is_checked) {
        enabled_controls(false);
        update_start_stop_button(QColor(Qt::red), "Stop");
        emit started(ui->lineEdit_ip->text(), ui->lineEdit_port->text());

    } else {
        update_start_stop_button(QColor(Qt::red), "Start");
        emit stopped();
        enabled_controls(true);
    }
}

void View::on_checkBox_accelerometer_stateChanged(int checkstate) {
    bool is_enabled{false};
    if (Qt::Checked == checkstate)
        is_enabled = true;
    else if (Qt::Unchecked == checkstate)
        is_enabled = false;

    emit accelerometer_toggled(is_enabled);
}

void View::on_checkBox_camera_stateChanged(int checkstate) {
    bool is_enabled{false};
    if (Qt::Checked == checkstate)
        is_enabled = true;
    else if (Qt::Unchecked == checkstate)
        is_enabled = false;

    emit camera_toggled(is_enabled);
}

void View::on_checkBox_gyroscope_stateChanged(int checkstate) {
    bool is_enabled{false};
    if (Qt::Checked == checkstate)
        is_enabled = true;
    else if (Qt::Unchecked == checkstate)
        is_enabled = false;

    emit gyroscope_toggled(is_enabled);
}

void View::on_checkBox_gps_stateChanged(int checkstate) {
    bool is_enabled{false};
    if (Qt::Checked == checkstate)
        is_enabled = true;
    else if (Qt::Unchecked == checkstate)
        is_enabled = false;

    emit gps_toggled(is_enabled);
}

void View::on_radioButton_1280_x_720_toggled(bool is_checked) {
    if (is_checked)
        emit resolution_hd();
}

void View::on_radioButton_640_x_480_toggled(bool is_checked) {
    if (is_checked)
        emit resolution_vga();
}

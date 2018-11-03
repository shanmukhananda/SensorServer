#include "common/pch.h"

#include "common/settings.h"
#include "presenter/presenter.h"
#include "ui_view.h"
#include "view/view.h"

View::View() {
    LOG_SCOPE;
    init();
}

void View::init() {
    LOG_SCOPE;
    _ui = std::make_unique<Ui::View>();
    _ui->setupUi(this);
    update_start_stop_button(QColor(Qt::green), "Start");
}

View::~View() {
    LOG_SCOPE;
}

void View::enable_controls(bool is_enabled_) {
    LOG_SCOPE;
    _ui->lineEdit_ip->setEnabled(is_enabled_);
    _ui->lineEdit_port->setEnabled(is_enabled_);

    _ui->radioButton_640_x_480->setEnabled(is_enabled_);
    _ui->radioButton_1280_x_720->setEnabled(is_enabled_);

    _ui->checkBox_accelerometer->setEnabled(is_enabled_);
    _ui->checkBox_camera->setEnabled(is_enabled_);
    _ui->checkBox_gps->setEnabled(is_enabled_);
    _ui->checkBox_gyroscope->setEnabled(is_enabled_);
}

void View::update_start_stop_button(QColor color, QString text) {
    LOG_SCOPE;
    auto qss = QString("background-color: %1").arg(color.name());
    _ui->pushButton_start_stop->setStyleSheet(qss);
    _ui->pushButton_start_stop->setText(text);
}

void View::update_view(Settings* settings_) {
    LOG_SCOPE;
    if (settings_->is_camera_enabled())
        _ui->checkBox_camera->setCheckState(Qt::Checked);
    else
        _ui->checkBox_camera->setCheckState(Qt::Unchecked);

    if (settings_->is_acceleromter_enabled())
        _ui->checkBox_accelerometer->setCheckState(Qt::Checked);
    else
        _ui->checkBox_accelerometer->setCheckState(Qt::Unchecked);

    if (settings_->is_gyroscope_enabled())
        _ui->checkBox_gyroscope->setCheckState(Qt::Checked);
    else
        _ui->checkBox_gyroscope->setCheckState(Qt::Unchecked);

    if (settings_->is_gps_enabled())
        _ui->checkBox_gps->setCheckState(Qt::Checked);
    else
        _ui->checkBox_gps->setCheckState(Qt::Unchecked);

    _ui->lineEdit_ip->setText(settings_->ip());
    _ui->lineEdit_port->setText(settings_->port());
}

void View::on_pushButton_start_stop_toggled(bool is_checked_) {
    LOG_SCOPE;
    if (is_checked_) {
        enable_controls(false);
        update_start_stop_button(QColor(Qt::red), "Stop");
        emit start_sensor_server(_ui->lineEdit_ip->text(),
                                 _ui->lineEdit_port->text());
    } else {
        update_start_stop_button(QColor(Qt::green), "Start");
        emit stop_sensor_server();
        enable_controls(true);
    }
}

void View::on_checkBox_accelerometer_stateChanged(int checkstate_) {
    LOG_SCOPE;
    auto is_enabled = false;
    if (Qt::Checked == checkstate_)
        is_enabled = true;
    else if (Qt::Unchecked == checkstate_)
        is_enabled = false;

    emit accelerometer_toggled(is_enabled);
}

void View::on_checkBox_camera_stateChanged(int checkstate_) {
    LOG_SCOPE;
    auto is_enabled = false;
    if (Qt::Checked == checkstate_)
        is_enabled = true;
    else if (Qt::Unchecked == checkstate_)
        is_enabled = false;

    emit camera_toggled(is_enabled);
}

void View::on_checkBox_gyroscope_stateChanged(int checkstate_) {
    LOG_SCOPE;
    auto is_enabled = false;
    if (Qt::Checked == checkstate_)
        is_enabled = true;
    else if (Qt::Unchecked == checkstate_)
        is_enabled = false;

    emit gyroscope_toggled(is_enabled);
}

void View::on_checkBox_gps_stateChanged(int checkstate_) {
    LOG_SCOPE;
    auto is_enabled = false;
    if (Qt::Checked == checkstate_)
        is_enabled = true;
    else if (Qt::Unchecked == checkstate_)
        is_enabled = false;

    emit gps_toggled(is_enabled);
}

void View::on_radioButton_1280_x_720_toggled(bool is_checked_) {
    LOG_SCOPE;
    if (is_checked_)
        emit resolution_hd();
}

void View::on_radioButton_640_x_480_toggled(bool is_checked_) {
    LOG_SCOPE;
    if (is_checked_)
        emit resolution_vga();
}

void View::model_initialized(QObject* filter_, Settings* settings_) {
    LOG_SCOPE;
    update_view(settings_);

    auto root_context = _ui->quickWidget_camera->rootContext();
    root_context->setContextProperty("video_filter", filter_);

    auto url = QUrl(QLatin1String("qrc:/camera.qml"));
    _ui->quickWidget_camera->setSource(url);

    auto root_object = _ui->quickWidget_camera->rootObject();
    auto qml_camera = root_object->findChild<QObject*>("camera");
    auto camera = qvariant_cast<QCamera*>(qml_camera->property("mediaObject"));
    camera->stop();

    show();
    emit view_initialized(camera);
}

void View::update_status(const QString& status_) {
    LOG_SCOPE;
    _ui->lineEdit_message->setText(status_);
}

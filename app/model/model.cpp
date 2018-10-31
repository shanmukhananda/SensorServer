#include "common/pch.h"

#include "common/settings.h"
#include "model/model.h"
#include "model/receiver.h"

Model::Model()
    : _receiver(std::make_unique<Receiver>())
    , _settings(std::make_unique<Settings>()) {
}

Model::~Model() {
}

void Model::start() {
    emit model_initalized(_receiver->get_video_filter(), _settings.get());
}

void Model::view_initialized(QCamera* camera_) {
    _receiver->set_camera(camera_);
}

void Model::started(QString ip_, QString port_) {
    _settings->ip(ip_);
    _settings->port(port_);
    _receiver->start_receive(_settings);
}

void Model::stopped() {
    _receiver->stop_receive();
}

void Model::resolution_vga() {
    _settings->image_resolution(resolution::vga);
}

void Model::resolution_hd() {
    _settings->image_resolution(resolution::hd);
}

void Model::accelerometer_toggled(bool is_enabled_) {
    _settings->enable_accelerometer(is_enabled_);
}

void Model::gyroscope_toggled(bool is_enabled_) {
    _settings->enable_gyroscope(is_enabled_);
}

void Model::gps_toggled(bool is_enabled_) {
    _settings->enable_gps(is_enabled_);
}

void Model::camera_toggled(bool is_enabled_) {
    _settings->enable_camera(is_enabled_);
}

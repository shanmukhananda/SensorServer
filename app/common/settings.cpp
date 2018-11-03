#include "common/pch.h"

#include "common/settings.h"

Settings::Settings()
    : _resolution(resolution::vga)
    , _is_accelerometer_enabled(false)
    , _is_gyroscope_enabled(false)
    , _is_camera_enabled(false)
    , _is_gps_enabled(false)
    , _ip()
    , _port() {
    LOG_SCOPE;
}

Settings::~Settings() {
    LOG_SCOPE;
}

void Settings::ip(const QString& ip_) {
    _ip = ip_;
}

QString Settings::ip() const {
    return _ip;
}

void Settings::port(const QString& port_) {
    _port = port_;
}

QString Settings::port() const {
    return _port;
}

void Settings::image_resolution(resolution resolution_) {
    _resolution = resolution_;
}

resolution Settings::image_resolution() const {
    return _resolution;
}

void Settings::enable_accelerometer(bool is_enabled_) {
    _is_accelerometer_enabled = is_enabled_;
}

bool Settings::is_acceleromter_enabled() const {
    return _is_accelerometer_enabled;
}

void Settings::enable_gyroscope(bool is_enabled_) {
    _is_gyroscope_enabled = is_enabled_;
}

bool Settings::is_gyroscope_enabled() const {
    return _is_gyroscope_enabled;
}

void Settings::enable_camera(bool is_enabled_) {
    _is_camera_enabled = is_enabled_;
}

bool Settings::is_camera_enabled() const {
    return _is_camera_enabled;
}

void Settings::enable_gps(bool is_enabled_) {
    _is_gps_enabled = is_enabled_;
}

bool Settings::is_gps_enabled() const {
    return _is_gps_enabled;
}

int Settings::imu_frequency() const {
    return 100;
}

qreal Settings::camera_frequency() const {
    return 30.0;
}

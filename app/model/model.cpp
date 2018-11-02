#include "common/pch.h"

#include "common/settings.h"
#include "model/model.h"
#include "model/receiver.h"
#include "model/transmitter.h"
#include "model/videofilter.h"

Model::Model() {
    init();
}

void Model::init() {
    _receiver = std::make_unique<Receiver>();
    _settings = std::make_unique<Settings>();
    _transmitter = std::make_unique<Transmitter>();

    auto connected = false;
    connected =
        connect(_receiver.get(),
                SIGNAL(received_sensordata(std::shared_ptr<SensorData>)),
                _transmitter.get(),
                SLOT(received_sensordata(std::shared_ptr<SensorData>)));
    Q_ASSERT(connected);

    connected =
        connect(_receiver.get(), SIGNAL(receiver_initialized(VideoFilter*)),
                this, SLOT(receiver_initialized(VideoFilter*)));
    Q_ASSERT(connected);

    connected =
        connect(this, SIGNAL(start_reception(const std::unique_ptr<Settings>&)),
                _receiver.get(),
                SLOT(start_reception(const std::unique_ptr<Settings>&)));
    Q_ASSERT(connected);

    connected = connect(this, SIGNAL(stop_reception()), _receiver.get(),
                        SLOT(stop_reception()));
    Q_ASSERT(connected);

    connected =
        connect(this,
                SIGNAL(start_transmission(const std::unique_ptr<Settings>&)),
                _transmitter.get(),
                SLOT(start_transmission(const std::unique_ptr<Settings>&)));
    Q_ASSERT(connected);

    connected = connect(this, SIGNAL(stop_transmission()), _transmitter.get(),
                        SLOT(stop_transmission()));
    Q_ASSERT(connected);

    connected = connect(this, SIGNAL(update_camera(QCamera*)), _receiver.get(),
                        SLOT(update_camera(QCamera*)));
    Q_ASSERT(connected);
    Q_UNUSED(connected);
}

Model::~Model() {
}

void Model::run() {
    _receiver->run();
}

void Model::view_initialized(QCamera* camera_) {
    emit update_camera(camera_);
}

void Model::receiver_initialized(VideoFilter* video_filter_) {
    emit model_initialized(video_filter_, _settings.get());
}

void Model::started_sensor_server(const QString& ip_, const QString& port_) {
    _settings->ip(ip_);
    _settings->port(port_);

    emit start_reception(_settings);
    emit start_transmission(_settings);
}

void Model::stopped_sensor_server() {
    emit stop_reception();
    emit stop_transmission();
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

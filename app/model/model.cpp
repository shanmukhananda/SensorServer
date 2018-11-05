#include "app/common/pch.h"

#include "app/common/settings.h"
#include "app/model/model.h"
#include "app/model/receiver.h"
#include "app/model/transmitter.h"
#include "app/model/videofilter.h"

Model::Model() {
    LOG_SCOPE;
    init();
}

void Model::init() {
    LOG_SCOPE;
    _receiver = std::make_unique<Receiver>();
    _settings = std::make_unique<Settings>();
    _transmitter = std::make_unique<Transmitter>();
    _receiver_thread = std::make_unique<QThread>();
    _transmitter_thread = std::make_unique<QThread>();

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

    connected = connect(this, SIGNAL(start_reception(Settings*)),
                        _receiver.get(), SLOT(start_reception(Settings*)));
    Q_ASSERT(connected);

    connected = connect(this, SIGNAL(stop_reception()), _receiver.get(),
                        SLOT(stop_reception()));
    Q_ASSERT(connected);

    connected =
        connect(this, SIGNAL(start_transmission(Settings*)), _transmitter.get(),
                SLOT(start_transmission(Settings*)));
    Q_ASSERT(connected);

    connected = connect(this, SIGNAL(stop_transmission()), _transmitter.get(),
                        SLOT(stop_transmission()));
    Q_ASSERT(connected);

    connected = connect(this, SIGNAL(update_camera(QCamera*)), _receiver.get(),
                        SLOT(update_camera(QCamera*)));

    connected = connect(_transmitter.get(), SIGNAL(status(QString)), this,
                        SIGNAL(status(QString)));
    Q_ASSERT(connected);
    Q_UNUSED(connected);

    _receiver->moveToThread(_receiver_thread.get());
    _transmitter->moveToThread(_transmitter_thread.get());
    _receiver_thread->start();
    _transmitter_thread->start();
}

Model::~Model() {
    LOG_SCOPE;

    _receiver_thread->quit();
    _receiver_thread->wait();

    _transmitter_thread->quit();
    _transmitter_thread->wait();
}

void Model::run() {
    LOG_SCOPE;
    _receiver->run();
}

void Model::view_initialized(QCamera* camera_) {
    LOG_SCOPE;
    emit update_camera(camera_);
}

void Model::receiver_initialized(VideoFilter* video_filter_) {
    LOG_SCOPE;
    emit model_initialized(video_filter_, _settings.get());
}

void Model::started_sensor_server(const QString& ip_, const QString& port_) {
    LOG_SCOPE;
    _settings->ip(ip_);
    _settings->port(port_);

    emit start_reception(_settings.get());
    emit start_transmission(_settings.get());
}

void Model::stopped_sensor_server() {
    LOG_SCOPE;
    emit stop_reception();
    emit stop_transmission();
}

void Model::resolution_vga() {
    LOG_SCOPE;
    _settings->image_resolution(resolution::vga);
}

void Model::resolution_hd() {
    LOG_SCOPE;
    _settings->image_resolution(resolution::hd);
}

void Model::accelerometer_toggled(bool is_enabled_) {
    LOG_SCOPE;
    _settings->enable_accelerometer(is_enabled_);
}

void Model::gyroscope_toggled(bool is_enabled_) {
    LOG_SCOPE;
    _settings->enable_gyroscope(is_enabled_);
}

void Model::gps_toggled(bool is_enabled_) {
    LOG_SCOPE;
    _settings->enable_gps(is_enabled_);
}

void Model::camera_toggled(bool is_enabled_) {
    LOG_SCOPE;
    _settings->enable_camera(is_enabled_);
}

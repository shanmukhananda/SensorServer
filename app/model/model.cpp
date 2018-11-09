#include "app/common/pch.h"

#include "app/common/settings.h"
#include "app/model/camerareceiver.h"
#include "app/model/georeceiver.h"
#include "app/model/imureceiver.h"
#include "app/model/model.h"
#include "app/model/transmitter.h"

Model::Model() {
    LOG_SCOPE;
    init();
}

void Model::init() {
    LOG_SCOPE;
    _settings = std::make_unique<Settings>();
    _transmitter = std::make_unique<Transmitter>();
    _camera_receiver = std::make_unique<CameraReceiver>();
    _imu_receiver = std::make_unique<IMUReceiver>();
    _geo_receiver = std::make_unique<GeoReceiver>();
    // _camera_receiver_thread = std::make_unique<QThread>();
    _imu_receiver_thread = std::make_unique<QThread>();
    _geo_receiver_thread = std::make_unique<QThread>();
    _transmitter_thread = std::make_unique<QThread>();

    auto connected = false;
    connected =
        connect(_camera_receiver.get(),
                SIGNAL(received_sensordata(std::shared_ptr<SensorData>)),
                _transmitter.get(),
                SLOT(received_sensordata(std::shared_ptr<SensorData>)));
    Q_ASSERT(connected);
    connected =
        connect(_imu_receiver.get(),
                SIGNAL(received_sensordata(std::shared_ptr<SensorData>)),
                _transmitter.get(),
                SLOT(received_sensordata(std::shared_ptr<SensorData>)));
    Q_ASSERT(connected);
    connected =
        connect(_geo_receiver.get(),
                SIGNAL(received_sensordata(std::shared_ptr<SensorData>)),
                _transmitter.get(),
                SLOT(received_sensordata(std::shared_ptr<SensorData>)));
    Q_ASSERT(connected);
    connected =
        connect(this, SIGNAL(start_reception(Settings*)),
                _camera_receiver.get(), SLOT(start_reception(Settings*)));
    Q_ASSERT(connected);
    connected = connect(this, SIGNAL(start_reception(Settings*)),
                        _imu_receiver.get(), SLOT(start_reception(Settings*)));
    Q_ASSERT(connected);
    connected = connect(this, SIGNAL(start_reception(Settings*)),
                        _geo_receiver.get(), SLOT(start_reception(Settings*)));
    Q_ASSERT(connected);
    connected = connect(this, SIGNAL(stop_reception()), _camera_receiver.get(),
                        SLOT(stop_reception()));
    Q_ASSERT(connected);
    connected = connect(this, SIGNAL(stop_reception()), _imu_receiver.get(),
                        SLOT(stop_reception()));
    Q_ASSERT(connected);
    connected = connect(this, SIGNAL(stop_reception()), _geo_receiver.get(),
                        SLOT(stop_reception()));
    Q_ASSERT(connected);
    connected =
        connect(this, SIGNAL(start_transmission(Settings*)), _transmitter.get(),
                SLOT(start_transmission(Settings*)));
    Q_ASSERT(connected);
    connected = connect(this, SIGNAL(stop_transmission()), _transmitter.get(),
                        SLOT(stop_transmission()));
    Q_ASSERT(connected);
    connected = connect(this, SIGNAL(update_camera(QCamera*)),
                        _camera_receiver.get(), SLOT(update_camera(QCamera*)));
    Q_ASSERT(connected);
    connected = connect(_transmitter.get(), SIGNAL(status(QString)), this,
                        SIGNAL(status(QString)));
    Q_ASSERT(connected);
    Q_UNUSED(connected);

    // _camera_receiver->moveToThread(_camera_receiver_thread.get());
    _imu_receiver->moveToThread(_imu_receiver_thread.get());
    _geo_receiver->moveToThread(_geo_receiver_thread.get());
    _transmitter->moveToThread(_transmitter_thread.get());

    // _camera_receiver_thread->start();
    _imu_receiver_thread->start();
    _geo_receiver_thread->start();
    _transmitter_thread->start();
}

Model::~Model() {
    LOG_SCOPE;

    // _camera_receiver_thread->quit();
    // _camera_receiver_thread->wait();
    _imu_receiver_thread->quit();
    _imu_receiver_thread->wait();
    _geo_receiver_thread->quit();
    _geo_receiver_thread->wait();
    _transmitter_thread->quit();
    _transmitter_thread->wait();
}

void Model::run() {
    LOG_SCOPE;
    emit model_initialized(_settings.get());
}

void Model::view_initialized(QCamera* camera_) {
    LOG_SCOPE;
    emit update_camera(camera_);
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

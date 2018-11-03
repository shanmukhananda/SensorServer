#include "common/pch.h"

#include "common/settings.h"
#include "model/receiver.h"
#include "model/sensordata.h"
#include "model/videofilter.h"

Receiver::Receiver() {
    LOG_SCOPE;
    init();
}

void Receiver::init() {
    LOG_SCOPE;
    _video_filter = std::make_unique<VideoFilter>(this);
    _timer = std::make_unique<QTimer>(this);
    _accelerometer = std::make_unique<QAccelerometer>(this);
    _gyroscope = std::make_unique<QGyroscope>(this);
    _geo_source = QGeoPositionInfoSource::createDefaultSource(this);

    auto connected = false;
    connected =
        connect(_video_filter.get(),
                SIGNAL(received_sensordata(std::shared_ptr<SensorData>)), this,
                SIGNAL(received_sensordata(std::shared_ptr<SensorData>)));
    Q_ASSERT(connected);

    connected = connect(_timer.get(), SIGNAL(timeout()), this, SLOT(timeout()));
    Q_ASSERT(connected);

    if (_geo_source)
        connect(_geo_source, SIGNAL(positionUpdated(QGeoPositionInfo)), this,
                SLOT(position_updated(QGeoPositionInfo)));

    Q_UNUSED(connected);
}

void Receiver::start_camera(Settings* settings_) {
    LOG_SCOPE;
    if (settings_->is_camera_enabled()) {
        QCameraViewfinderSettings vfsettings;
        vfsettings.setMinimumFrameRate(settings_->camera_frequency());
        vfsettings.setMaximumFrameRate(settings_->camera_frequency());
        QSize res;
        if (resolution::hd == settings_->image_resolution())
            res = QSize(1280, 720);
        else
            res = QSize(640, 480);
        vfsettings.setResolution(res);
        _camera->setViewfinderSettings(vfsettings);
        _camera->start();
    } else
        _camera->stop();
}

void Receiver::start_geosource(Settings* settings_) {
    LOG_SCOPE;
    if (_geo_source) {
        if (settings_->is_gps_enabled())
            _geo_source->startUpdates();
        else
            _geo_source->stopUpdates();
    }
}

void Receiver::start_imu(Settings* settings_) {
    LOG_SCOPE;
    if (settings_->is_acceleromter_enabled())
        _accelerometer->start();
    else
        _accelerometer->stop();

    if (settings_->is_gyroscope_enabled())
        _gyroscope->start();
    else
        _gyroscope->stop();

    if (settings_->is_acceleromter_enabled() ||
        settings_->is_gyroscope_enabled())
        _timer->start(1000 / settings_->imu_frequency());
    else
        _timer->stop();
}

void Receiver::send_accelerometer_reading() {
    // LOG_SCOPE;
    auto accel_reading = _accelerometer->reading();
    if (accel_reading) {
        auto timestamp = QDateTime::currentMSecsSinceEpoch();
        Q_ASSERT(timestamp > 0);
        auto accel_data = std::make_shared<AcceleromterData>();
        accel_data->timestamp = static_cast<std::uint64_t>(timestamp);
        accel_data->x = accel_reading->x();
        accel_data->y = accel_reading->y();
        accel_data->z = accel_reading->z();

        emit received_sensordata(accel_data);
    }
}

void Receiver::send_gyroscope_reading() {
    // LOG_SCOPE;
    auto gyro_reading = _gyroscope->reading();
    if (gyro_reading) {
        auto timestamp = QDateTime::currentMSecsSinceEpoch();
        Q_ASSERT(timestamp > 0);
        auto gyro_data = std::make_shared<GyroscopeData>();
        gyro_data->timestamp = static_cast<std::uint64_t>(timestamp);
        gyro_data->x = gyro_reading->x();
        gyro_data->y = gyro_reading->y();
        gyro_data->z = gyro_reading->z();

        emit received_sensordata(gyro_data);
    }
}

Receiver::~Receiver() {
    LOG_SCOPE;
}

void Receiver::run() {
    LOG_SCOPE;
    emit receiver_initialized(_video_filter.get());
}

void Receiver::update_camera(QCamera* camera_) {
    LOG_SCOPE;
    _camera = camera_;
    _camera->focus()->setFocusMode(QCameraFocus::FocusMode::InfinityFocus);
}

void Receiver::start_reception(Settings* settings_) {
    LOG_SCOPE;

    start_camera(settings_);
    start_geosource(settings_);
    start_imu(settings_);
}

void Receiver::stop_reception() {
    LOG_SCOPE;
    _camera->stop();
    if (_geo_source)
        _geo_source->stopUpdates();
    _timer->stop();
}

void Receiver::timeout() {
    // LOG_SCOPE;
    send_accelerometer_reading();
    send_gyroscope_reading();
}

void Receiver::position_updated(const QGeoPositionInfo& position_) {
    // LOG_SCOPE;
    auto timestamp = QDateTime::currentMSecsSinceEpoch();
    Q_ASSERT(timestamp > 0);

    auto geo_data = std::make_shared<GeodeticData>();
    geo_data->latitude = position_.coordinate().latitude();
    geo_data->longitude = position_.coordinate().longitude();
    geo_data->altitude = position_.coordinate().altitude();

    if (position_.hasAttribute(QGeoPositionInfo::Direction))
        geo_data->direction = position_.attribute(QGeoPositionInfo::Direction);

    if (position_.hasAttribute(QGeoPositionInfo::HorizontalAccuracy))
        geo_data->horizontal_accuracy =
            position_.attribute(QGeoPositionInfo::HorizontalAccuracy);

    if (position_.hasAttribute(QGeoPositionInfo::GroundSpeed))
        geo_data->ground_speed =
            position_.attribute(QGeoPositionInfo::GroundSpeed);
}

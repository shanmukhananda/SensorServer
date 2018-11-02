#include "common/pch.h"

#include "common/settings.h"
#include "model/receiver.h"
#include "model/sensordata.h"
#include "model/videofilter.h"

Receiver::Receiver() {
    init();
}

void Receiver::init() {
    _video_filter = std::make_unique<VideoFilter>();
    _timer = std::make_unique<QTimer>(this);
    _accelerometer = std::make_unique<QAccelerometer>(this);
    _gyroscope = std::make_unique<QGyroscope>(this);
    _geo_source = QGeoPositionInfoSource::createDefaultSource(this);

    auto connected = false;
    connected =
        connect(_video_filter.get(), SIGNAL(received_videoframe(QVideoFrame*)),
                this, SLOT(received_videoframe(QVideoFrame*)));
    Q_ASSERT(connected);

    connected = connect(_timer.get(), SIGNAL(timeout()), this, SLOT(timeout()));
    Q_ASSERT(connected);

    if (_geo_source)
        connect(_geo_source, SIGNAL(positionUpdated(const QGeoPositionInfo&)),
                this, SLOT(position_updated(const QGeoPositionInfo&)));

    Q_UNUSED(connected);
}

Receiver::~Receiver() {
}

void Receiver::run() {
    emit receiver_initialized(_video_filter.get());
}

void Receiver::update_camera(QCamera* camera_) {
    _camera = camera_;
    _camera->focus()->setFocusMode(QCameraFocus::FocusMode::InfinityFocus);
}

void Receiver::start_reception(const std::unique_ptr<Settings>& settings_) {
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
    }

    else
        _camera->stop();

    if (_geo_source) {
        if (settings_->is_gps_enabled())
            _geo_source->startUpdates();
        else
            _geo_source->stopUpdates();
    }

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

void Receiver::stop_reception() {
    _camera->stop();
    if (_geo_source)
        _geo_source->stopUpdates();
    _timer->stop();
}

void Receiver::received_videoframe(QVideoFrame* frame_) {
    qDebug() << QThread::currentThreadId() << __FUNCTION__;
    auto timestamp = QDateTime::currentMSecsSinceEpoch();
    QString pix_format;
    QDebug(&pix_format) << frame_->pixelFormat();
    pix_format = pix_format.trimmed();

    Q_ASSERT(frame_->width() > 0);
    Q_ASSERT(frame_->height() > 0);
    Q_ASSERT(timestamp > 0);

    auto image_data = std::make_shared<ImageData>();
    image_data->timestamp = static_cast<std::uint64_t>(timestamp);
    image_data->width = static_cast<std::uint16_t>(frame_->width());
    image_data->height = static_cast<std::uint16_t>(frame_->height());
    image_data->pixel_format = pix_format.toStdString();

    frame_->map(QAbstractVideoBuffer::ReadOnly);
    auto size = frame_->mappedBytes();
    auto data = frame_->bits();
    image_data->data.assign(data, data + size);
    frame_->unmap();

    emit received_sensordata(image_data);
}

void Receiver::timeout() {
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

void Receiver::position_updated(const QGeoPositionInfo& position_) {
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

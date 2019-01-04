#include "sensorserver/common/pch.h"

#include "sensorserver/common/settings.h"
#include "sensorserver/model/imureceiver.h"
#include "sensorserver/model/sensordata.h"

IMUReceiver::IMUReceiver() {
    LOG_SCOPE;
    init();
}

void IMUReceiver::init() {
    _timer = std::make_unique<QTimer>(this);
    _accelerometer = std::make_unique<QAccelerometer>(this);
    _gyroscope = std::make_unique<QGyroscope>(this);

    auto connected = false;
    connected = connect(_timer.get(), SIGNAL(timeout()), this, SLOT(timeout()));
    Q_ASSERT(connected);
    Q_UNUSED(connected);
}

IMUReceiver::~IMUReceiver() {
    LOG_SCOPE;
}

void IMUReceiver::start_reception(Settings* settings_) {
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

void IMUReceiver::stop_reception() {
    LOG_SCOPE;
    _timer->stop();
}

void IMUReceiver::timeout() {
    // LOG_SCOPE;
    send_accelerometer_reading();
    send_gyroscope_reading();
}

void IMUReceiver::send_accelerometer_reading() {
    // LOG_SCOPE;
    auto accel_reading = _accelerometer->reading();
    if (accel_reading) {
        auto timestamp = QDateTime::currentMSecsSinceEpoch();
        Q_ASSERT(timestamp > 0);
        auto accel_data = std::make_shared<AcceleromterData>();
        accel_data->_timestamp = static_cast<std::uint64_t>(timestamp);
        accel_data->x = accel_reading->x();
        accel_data->y = accel_reading->y();
        accel_data->z = accel_reading->z();

        emit received_sensordata(accel_data);
    }
}

void IMUReceiver::send_gyroscope_reading() {
    // LOG_SCOPE;
    auto gyro_reading = _gyroscope->reading();
    if (gyro_reading) {
        auto timestamp = QDateTime::currentMSecsSinceEpoch();
        Q_ASSERT(timestamp > 0);
        auto gyro_data = std::make_shared<GyroscopeData>();
        gyro_data->_timestamp = static_cast<std::uint64_t>(timestamp);
        gyro_data->x = gyro_reading->x();
        gyro_data->y = gyro_reading->y();
        gyro_data->z = gyro_reading->z();

        emit received_sensordata(gyro_data);
    }
}

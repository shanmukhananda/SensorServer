#pragma once

#include "app/common/pch.h"

class Settings;
class SensorData;

class IMUReceiver : public QObject {
    Q_OBJECT
public:
    IMUReceiver();
    ~IMUReceiver() override;

signals:
    void received_sensordata(std::shared_ptr<SensorData>);

public slots:
    void start_reception(Settings*);
    void stop_reception();
    void timeout();

private:
    void init();
    void send_accelerometer_reading();
    void send_gyroscope_reading();

    std::unique_ptr<QAccelerometer> _accelerometer;
    std::unique_ptr<QGyroscope> _gyroscope;
    std::unique_ptr<QTimer> _timer;
};

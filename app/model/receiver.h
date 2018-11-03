#pragma once

#include "common/pch.h"

class VideoFilter;
class Settings;
class SensorData;

class Receiver : public QObject {
    Q_OBJECT

public:
    Receiver();
    ~Receiver() override;
    void run();

signals:
    void received_sensordata(std::shared_ptr<SensorData>);
    void receiver_initialized(VideoFilter*);

private slots:
    void update_camera(QCamera*);
    void start_reception(Settings*);
    void stop_reception();
    void timeout();
    void position_updated(const QGeoPositionInfo&);

private:
    void init();
    void start_camera(Settings*);
    void start_geosource(Settings*);
    void start_imu(Settings*);
    void send_accelerometer_reading();
    void send_gyroscope_reading();

    std::unique_ptr<VideoFilter> _video_filter;
    std::unique_ptr<QTimer> _timer;
    QCamera* _camera{nullptr};
    QGeoPositionInfoSource* _geo_source{nullptr};
    std::unique_ptr<QAccelerometer> _accelerometer;
    std::unique_ptr<QGyroscope> _gyroscope;
};

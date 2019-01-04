#pragma once

#include "sensorserver/common/pch.h"

class CameraReceiver;
class IMUReceiver;
class GeoReceiver;
class Settings;
class Transmitter;
class Compressor;

class Model : public QObject {
    Q_OBJECT

public:
    Model();
    ~Model() override;
    void run();

signals:
    void model_initialized(Settings*);
    void update_camera(QCamera*);
    void start_reception(Settings*);
    void stop_reception();
    void start_transmission(Settings*);
    void stop_transmission();
    void status(const QString&);

private slots:
    void view_initialized(QCamera*);
    void started_sensor_server(const QString&, const QString&);
    void stopped_sensor_server();
    void resolution_vga();
    void resolution_hd();
    void accelerometer_toggled(bool);
    void gyroscope_toggled(bool);
    void gps_toggled(bool);
    void camera_toggled(bool);

private:
    void init();

    std::unique_ptr<Settings> _settings;
    std::unique_ptr<CameraReceiver> _camera_receiver;
    std::unique_ptr<IMUReceiver> _imu_receiver;
    std::unique_ptr<GeoReceiver> _geo_receiver;
    std::unique_ptr<Transmitter> _transmitter;
    std::unique_ptr<Compressor> _compressor;
    std::unique_ptr<QThread> _camera_receiver_thread;
    std::unique_ptr<QThread> _imu_receiver_thread;
    std::unique_ptr<QThread> _geo_receiver_thread;
    std::unique_ptr<QThread> _transmitter_thread;
    std::unique_ptr<QThread> _compressor_thread;
};

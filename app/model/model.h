#pragma once

#include "app/common/pch.h"

class Receiver;
class Settings;
class Transmitter;
class VideoFilter;

class Model : public QObject {
    Q_OBJECT

public:
    Model();
    ~Model() override;
    void run();

signals:
    void model_initialized(QObject*, Settings*);
    void update_camera(QCamera*);
    void start_reception(Settings*);
    void stop_reception();
    void start_transmission(Settings*);
    void stop_transmission();
    void status(const QString&);

private slots:
    void view_initialized(QCamera*);
    void receiver_initialized(VideoFilter*);
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

    std::unique_ptr<Receiver> _receiver;
    std::unique_ptr<Settings> _settings;
    std::unique_ptr<Transmitter> _transmitter;
    std::unique_ptr<QThread> _receiver_thread;
    std::unique_ptr<QThread> _transmitter_thread;
};

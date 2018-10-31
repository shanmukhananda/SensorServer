#pragma once

#include "common/pch.h"

enum class resolution { vga, hd };

class Settings : public QObject {
    Q_OBJECT

public:
    Settings();
    ~Settings() override;

    void ip(const QString&);
    QString ip() const;

    void port(const QString&);
    QString port() const;

    void image_resolution(resolution);
    resolution image_resolution() const;

    void enable_accelerometer(bool);
    bool is_acceleromter_enabled() const;

    void enable_gyroscope(bool);
    bool is_gyroscope_enabled() const;

    void enable_camera(bool);
    bool is_camera_enabled() const;

    void enable_gps(bool);
    bool is_gps_enabled() const;

    int imu_frequency() const;
    qreal camera_frequency() const;

private:
    resolution _resolution;
    bool _is_accelerometer_enabled;
    bool _is_gyroscope_enabled;
    bool _is_camera_enabled;
    bool _is_gps_enabled;
    QString _ip;
    QString _port;
};

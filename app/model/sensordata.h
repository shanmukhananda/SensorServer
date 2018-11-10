#pragma once

#include "app/common/pch.h"

enum class sensor_type : std::uint8_t {
    none,
    accelerometer,
    camera,
    geo,
    gyroscope,
};

class SensorData : public QObject {
public:
    SensorData() = default;
    virtual ~SensorData() = default;
    virtual QByteArray serialize() const = 0;
    virtual sensor_type type() const = 0;
    virtual std::uint64_t timestamp() const = 0;
    sensor_type _type = sensor_type::none;
    std::uint64_t _timestamp = 0;
};

class AcceleromterData : public SensorData {
public:
    AcceleromterData();
    ~AcceleromterData() override = default;
    QByteArray serialize() const override;
    sensor_type type() const override;
    std::uint64_t timestamp() const override;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

class GyroscopeData : public SensorData {
public:
    GyroscopeData();
    ~GyroscopeData() override = default;
    QByteArray serialize() const override;
    sensor_type type() const override;
    std::uint64_t timestamp() const override;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

class GeodeticData : public SensorData {
public:
    GeodeticData();
    ~GeodeticData() override = default;
    QByteArray serialize() const override;
    sensor_type type() const override;
    std::uint64_t timestamp() const override;
    double latitude = 0.0;
    double longitude = 0.0;
    double altitude = 0.0;
    double horizontal_accuracy = -1.0;
    double ground_speed = -1.0;
    double direction = -1.0;
};

class ImageData : public SensorData {
public:
    ImageData();
    ~ImageData() override = default;
    QByteArray serialize() const override;
    sensor_type type() const override;
    std::uint64_t timestamp() const override;
    std::int32_t width = 0;
    std::int32_t height = 0;
    std::string pixel_format;
    std::int32_t plane_count = 0;
    std::vector<std::int32_t> bytes_per_line_per_plane;
    std::int32_t mapped_bytes = 0;
    std::vector<std::uint8_t> bits;
};

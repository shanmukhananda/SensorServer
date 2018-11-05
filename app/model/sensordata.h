#pragma once

#include "app/common/pch.h"

enum class sensor_type : std::uint8_t {
    none,
    camera,
    accelerometer,
    gyroscope,
    geo
};

class SensorData : public QObject {
public:
    SensorData() = default;
    virtual ~SensorData() = default;
    virtual std::vector<std::uint8_t> serialize() const = 0;
    sensor_type type = sensor_type::none;
    std::uint64_t timestamp = 0;
};

class AcceleromterData : public SensorData {
public:
    AcceleromterData();
    ~AcceleromterData() override = default;
    std::vector<std::uint8_t> serialize() const override;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

class GyroscopeData : public SensorData {
public:
    GyroscopeData();
    ~GyroscopeData() override = default;
    std::vector<std::uint8_t> serialize() const override;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

class GeodeticData : public SensorData {
public:
    GeodeticData();
    ~GeodeticData() override = default;
    std::vector<std::uint8_t> serialize() const override;
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
    std::vector<std::uint8_t> serialize() const override;
    std::int32_t width = 0;
    std::int32_t height = 0;
    std::string pixel_format;
    std::vector<std::int32_t> bytes_per_line_per_plane;
    std::vector<std::uint8_t> bits;
};

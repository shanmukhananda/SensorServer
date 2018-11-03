#pragma once

#include "common/pch.h"

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
    sensor_type type = sensor_type::none;
    std::uint64_t timestamp = 0;
};

class AcceleromterData : public SensorData {
public:
    AcceleromterData();
    ~AcceleromterData() override = default;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

class GyroscopeData : public SensorData {
public:
    GyroscopeData();
    ~GyroscopeData() override = default;
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

class GeodeticData : public SensorData {
public:
    GeodeticData();
    ~GeodeticData() override = default;
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
    std::uint16_t width = 0;
    std::uint16_t height = 0;
    std::string pixel_format;
    std::vector<uint8_t> data;
};

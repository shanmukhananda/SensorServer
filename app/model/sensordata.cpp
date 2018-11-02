#include "common/pch.h"

#include "model/sensordata.h"

AcceleromterData::AcceleromterData() {
    type = sensor_type::accelerometer;
}

GyroscopeData::GyroscopeData() {
    type = sensor_type::gyroscope;
}

GeodeticData::GeodeticData() {
    type = sensor_type::geo;
}

ImageData::ImageData() {
    type = sensor_type::camera;
}

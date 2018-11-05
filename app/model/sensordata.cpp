#include "app/common/pch.h"

#include "app/model/sensordata.h"

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

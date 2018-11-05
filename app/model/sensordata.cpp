#include "app/common/pch.h"

#include "app/messages/messages_generated.h"
#include "app/model/sensordata.h"

AcceleromterData::AcceleromterData() {
    type = sensor_type::accelerometer;
}

std::vector<uint8_t> AcceleromterData::serialize() const {
    return std::vector<uint8_t>({'a', 'c', 'c'});
}

GyroscopeData::GyroscopeData() {
    type = sensor_type::gyroscope;
}

std::vector<uint8_t> GyroscopeData::serialize() const {
    return std::vector<uint8_t>({'g', 'y', 'r'});
}

GeodeticData::GeodeticData() {
    type = sensor_type::geo;
}

std::vector<uint8_t> GeodeticData::serialize() const {
    return std::vector<uint8_t>({'g', 'e', 'o'});
}

ImageData::ImageData() {
    type = sensor_type::camera;
}

std::vector<uint8_t> ImageData::serialize() const {
    return std::vector<uint8_t>({'c', 'a', 'm'});
}

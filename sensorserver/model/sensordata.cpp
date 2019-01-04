#include "sensorserver/common/pch.h"

#include "sensorserver/common/log.h"
#include "sensorserver/messages/messages_generated.h"
#include "sensorserver/model/sensordata.h"

AcceleromterData::AcceleromterData() {
    _type = sensor_type::accelerometer;
}

QByteArray AcceleromterData::serialize() const {
    // LOG_SCOPE;
    flatbuffers::FlatBufferBuilder fbb;
    messages::AccelerationBuilder builder(fbb);
    builder.add_timestamp(_timestamp);
    builder.add_type(messages::SensorType_Accelerometer);
    builder.add_x(x);
    builder.add_y(y);
    builder.add_z(z);
    fbb.Finish(builder.Finish());

    auto data = reinterpret_cast<const char*>(fbb.GetBufferPointer());
    auto size = static_cast<int>(fbb.GetSize());
    return QByteArray(data, size);
}

sensor_type AcceleromterData::type() const {
    return _type;
}

uint64_t AcceleromterData::timestamp() const {
    return _timestamp;
}

GyroscopeData::GyroscopeData() {
    _type = sensor_type::gyroscope;
}

QByteArray GyroscopeData::serialize() const {
    // LOG_SCOPE;
    flatbuffers::FlatBufferBuilder fbb;
    messages::OrientationBuilder builder(fbb);
    builder.add_timestamp(_timestamp);
    builder.add_type(messages::SensorType_Gyroscope);
    builder.add_x(x);
    builder.add_y(y);
    builder.add_z(z);
    fbb.Finish(builder.Finish());

    auto data = reinterpret_cast<const char*>(fbb.GetBufferPointer());
    auto size = static_cast<int>(fbb.GetSize());
    return QByteArray(data, size);
}

sensor_type GyroscopeData::type() const {
    return _type;
}

uint64_t GyroscopeData::timestamp() const {
    return _timestamp;
}

GeodeticData::GeodeticData() {
    _type = sensor_type::geo;
}

QByteArray GeodeticData::serialize() const {
    // LOG_SCOPE;
    flatbuffers::FlatBufferBuilder fbb;
    messages::GeolocationBuilder builder(fbb);
    builder.add_altitude(altitude);
    builder.add_direction(direction);
    builder.add_ground_speed(ground_speed);
    builder.add_horizontal_accuracy(horizontal_accuracy);
    builder.add_latitude(latitude);
    builder.add_longitude(longitude);
    builder.add_timestamp(_timestamp);
    builder.add_type(messages::SensorType_Geo);

    auto data = reinterpret_cast<const char*>(fbb.GetBufferPointer());
    auto size = static_cast<int>(fbb.GetSize());
    return QByteArray(data, size);
}

sensor_type GeodeticData::type() const {
    return _type;
}

uint64_t GeodeticData::timestamp() const {
    return _timestamp;
}

ImageData::ImageData() {
    _type = sensor_type::camera;
}

QByteArray ImageData::serialize() const {
    // LOG_SCOPE;
    flatbuffers::FlatBufferBuilder fbb;
    auto bits_fb = fbb.CreateVector(bits);
    messages::ImageBuilder builder(fbb);
    builder.add_bits(bits_fb);
    builder.add_type(messages::SensorType_Camera);
    builder.add_timestamp(_timestamp);
    fbb.Finish(builder.Finish());

    auto data = reinterpret_cast<const char*>(fbb.GetBufferPointer());
    auto size = static_cast<int>(fbb.GetSize());
    return QByteArray(data, size);
}

sensor_type ImageData::type() const {
    return _type;
}

uint64_t ImageData::timestamp() const {
    return _timestamp;
}

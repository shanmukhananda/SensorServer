#include "app/common/pch.h"

#include "app/common/log.h"
#include "app/messages/messages_generated.h"
#include "app/model/sensordata.h"

AcceleromterData::AcceleromterData() {
    _type = sensor_type::accelerometer;
}

std::vector<std::uint8_t> AcceleromterData::serialize() const {
    // LOG_SCOPE;
    flatbuffers::FlatBufferBuilder fbb;
    messages::AccelerationBuilder builder(fbb);
    builder.add_timestamp(_timestamp);
    builder.add_type(messages::SensorType_Accelerometer);
    builder.add_x(x);
    builder.add_y(y);
    builder.add_z(z);
    fbb.Finish(builder.Finish());

    std::vector<std::uint8_t> serialized;
    serialized.reserve(fbb.GetSize());
    serialized.assign(fbb.GetBufferPointer(),
                      fbb.GetBufferPointer() + fbb.GetSize());
    return serialized;
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

std::vector<std::uint8_t> GyroscopeData::serialize() const {
    // LOG_SCOPE;
    flatbuffers::FlatBufferBuilder fbb;
    messages::OrientationBuilder builder(fbb);
    builder.add_timestamp(_timestamp);
    builder.add_type(messages::SensorType_Gyroscope);
    builder.add_x(x);
    builder.add_y(y);
    builder.add_z(z);
    fbb.Finish(builder.Finish());

    std::vector<std::uint8_t> serialized;
    serialized.reserve(fbb.GetSize());
    serialized.assign(fbb.GetBufferPointer(),
                      fbb.GetBufferPointer() + fbb.GetSize());
    return serialized;
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

std::vector<std::uint8_t> GeodeticData::serialize() const {
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

    std::vector<std::uint8_t> serialized;
    serialized.reserve(fbb.GetSize());
    serialized.assign(fbb.GetBufferPointer(),
                      fbb.GetBufferPointer() + fbb.GetSize());
    return serialized;
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

std::vector<std::uint8_t> ImageData::serialize() const {
    // LOG_SCOPE;
    flatbuffers::FlatBufferBuilder fbb;
    auto bits_fb = fbb.CreateVector(bits);
    auto pixel_format_fb = fbb.CreateString(pixel_format);
    auto bplpl_fb = fbb.CreateVector(bytes_per_line_per_plane);
    messages::ImageBuilder builder(fbb);
    builder.add_bits(bits_fb);
    builder.add_bytes_per_line_per_plane(bplpl_fb);
    builder.add_height(height);
    builder.add_mapped_bytes(mapped_bytes);
    builder.add_pixel_format(pixel_format_fb);
    builder.add_plane_count(plane_count);
    builder.add_type(messages::SensorType_Camera);
    builder.add_width(width);
    fbb.Finish(builder.Finish());

    std::vector<std::uint8_t> serialized;
    serialized.reserve(fbb.GetSize());
    serialized.assign(fbb.GetBufferPointer(),
                      fbb.GetBufferPointer() + fbb.GetSize());
    return serialized;
}

sensor_type ImageData::type() const {
    return _type;
}

uint64_t ImageData::timestamp() const {
    return _timestamp;
}

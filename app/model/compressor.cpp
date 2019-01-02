#include "app/common/pch.h"

#include "app/model/compressor.h"
#include "app/model/sensordata.h"

Compressor::Compressor() {
    LOG_SCOPE;
}

Compressor::~Compressor() {
    LOG_SCOPE;
}

void Compressor::received_image(qint64 timestamp_,
                                std::shared_ptr<QImage> frame_) {
    // LOG_SCOPE;
    QByteArray bytearray;
    QBuffer buffer(&bytearray);
    buffer.open(QIODevice::WriteOnly);
    frame_->save(&buffer, "JPG", 100);

    auto image_data = std::make_shared<ImageData>();
    image_data->_timestamp = static_cast<std::uint64_t>(timestamp_);

    Q_ASSERT(bytearray.size() > -1);
    auto size = static_cast<std::size_t>(bytearray.size());
    image_data->bits.reserve(size);
    auto begin = bytearray.data();
    image_data->bits.assign(begin, begin + size);

    emit received_sensordata(image_data);
}

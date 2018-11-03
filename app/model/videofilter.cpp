#include "common/pch.h"

#include "model/sensordata.h"
#include "model/videofilter.h"

VideoFilterRunnable::VideoFilterRunnable(VideoFilter* filter_)
    : _filter(filter_) {
    LOG_SCOPE;
}

QVideoFrame
VideoFilterRunnable::run(QVideoFrame* frame_, const QVideoSurfaceFormat&,
                         QVideoFilterRunnable::RunFlags) {
    // LOG_SCOPE;
    _filter->on_videoframe(frame_);
    return *frame_;
}

VideoFilter::VideoFilter(QObject* parent) : QAbstractVideoFilter(parent) {
    LOG_SCOPE;
}

QVideoFilterRunnable* VideoFilter::createFilterRunnable() {
    LOG_SCOPE;
    return new VideoFilterRunnable(this);
}

void VideoFilter::on_videoframe(QVideoFrame* frame_) {
    // LOG_SCOPE;
    if (!frame_ || !frame_->isValid())
        return;

    auto timestamp = QDateTime::currentMSecsSinceEpoch();
    QString pix_format;
    QDebug(&pix_format) << frame_->pixelFormat();
    pix_format = pix_format.trimmed();

    Q_ASSERT(frame_->width() > 0);
    Q_ASSERT(frame_->height() > 0);
    Q_ASSERT(timestamp > 0);

    auto image_data = std::make_shared<ImageData>();
    image_data->timestamp = static_cast<std::uint64_t>(timestamp);
    image_data->width = static_cast<std::uint16_t>(frame_->width());
    image_data->height = static_cast<std::uint16_t>(frame_->height());
    image_data->pixel_format = pix_format.toStdString();

    frame_->map(QAbstractVideoBuffer::ReadOnly);
    auto size = frame_->mappedBytes();
    auto data = frame_->bits();
    image_data->data.assign(data, data + size);
    frame_->unmap();

    emit received_sensordata(image_data);
}

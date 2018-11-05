#include "app/common/pch.h"

#include "app/model/sensordata.h"
#include "app/model/videofilter.h"

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

    frame_->map(QAbstractVideoBuffer::ReadOnly);

    image_data->width = frame_->width();
    image_data->height = frame_->height();
    image_data->pixel_format = pix_format.toStdString();

    auto nplanes = frame_->planeCount();
    for (decltype(nplanes) i = 0; i < nplanes; ++i)
        image_data->bytes_per_line_per_plane.push_back(frame_->bytesPerLine(i));

    image_data->bits.assign(frame_->bits(),
                            frame_->bits() + frame_->mappedBytes());

    frame_->unmap();

    emit received_sensordata(image_data);
}

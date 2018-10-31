#include "common/pch.h"

#include "model/videofilter.h"

VideoFilterRunnable::VideoFilterRunnable(VideoFilter* filter_)
    : _filter(filter_) {
}

QVideoFrame
VideoFilterRunnable::run(QVideoFrame* frame_, const QVideoSurfaceFormat&,
                         QVideoFilterRunnable::RunFlags) {
    if (!frame_->isValid())
        return *frame_;

    _filter->on_videoframe(frame_);
    return *frame_;
}

QVideoFilterRunnable* VideoFilter::createFilterRunnable() {
    return new VideoFilterRunnable(this);
}

void VideoFilter::on_videoframe(QVideoFrame* frame_) {
    static std::size_t count = 0;
    qDebug()
        << "Timestamp:"
        << static_cast<quint64>(QDateTime::currentMSecsSinceEpoch())
        << "Received frame:" << ++count
        << "pixelformat=" << frame_->pixelFormat() << "size=" << frame_->size();
    emit received_videoframe(frame_);
}

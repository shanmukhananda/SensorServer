#include "common/pch.h"

#include "model/videofilter.h"

VideoFilterRunnable::VideoFilterRunnable(VideoFilter* filter_)
    : _filter(filter_) {
    LOG_SCOPE;
}

QVideoFrame
VideoFilterRunnable::run(QVideoFrame* frame_, const QVideoSurfaceFormat&,
                         QVideoFilterRunnable::RunFlags) {
    // LOG_SCOPE;
    if (!frame_->isValid())
        return *frame_;

    _filter->on_videoframe(frame_);
    return *frame_;
}

QVideoFilterRunnable* VideoFilter::createFilterRunnable() {
    LOG_SCOPE;
    return new VideoFilterRunnable(this);
}

void VideoFilter::on_videoframe(QVideoFrame* frame_) {
    // LOG_SCOPE;
    emit received_videoframe(frame_);
}

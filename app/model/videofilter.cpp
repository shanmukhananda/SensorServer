#include "common/pch.h"

#include "model/videofilter.h"

QVideoFrame
VideoFilterRunnable::run(QVideoFrame* frame, const QVideoSurfaceFormat&,
                         QVideoFilterRunnable::RunFlags) {
    static std::size_t count = 0;
    qDebug()
        << "Received frame:" << ++count
        << "pixelformat=" << frame->pixelFormat() << "size=" << frame->size();
    return *frame;
}

QVideoFilterRunnable* VideoFilter::createFilterRunnable() {
    return new VideoFilterRunnable();
}

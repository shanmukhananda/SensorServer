#pragma once

#include "common/pch.h"

class VideoFilter;

class VideoFilterRunnable : public QVideoFilterRunnable {
public:
    explicit VideoFilterRunnable(VideoFilter*);
    QVideoFrame
    run(QVideoFrame*, const QVideoSurfaceFormat&, RunFlags) override;

private:
    VideoFilter* _filter{nullptr};
};

class VideoFilter : public QAbstractVideoFilter {
    Q_OBJECT

public:
    virtual ~VideoFilter() = default;
    QVideoFilterRunnable* createFilterRunnable();
    void on_videoframe(QVideoFrame*);

signals:
    void finished(QObject*);
    void received_videoframe(QVideoFrame*);
};

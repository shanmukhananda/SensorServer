#pragma once

#include "common/pch.h"

class VideoFilterRunnable : public QVideoFilterRunnable {
public:
    QVideoFrame
    run(QVideoFrame*, const QVideoSurfaceFormat&, RunFlags) override;
};

class VideoFilter : public QAbstractVideoFilter {
public:
    QVideoFilterRunnable* createFilterRunnable();
signals:
    void finished(QObject*);
};

#pragma once

#include "common/pch.h"

class VideoFilter;
class SensorData;

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
    explicit VideoFilter(QObject* parent = nullptr);
    virtual ~VideoFilter() override = default;
    QVideoFilterRunnable* createFilterRunnable() override;
    void on_videoframe(QVideoFrame*);

signals:
    void finished(QObject*);
    void received_sensordata(std::shared_ptr<SensorData>);
};

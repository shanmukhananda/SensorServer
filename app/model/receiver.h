#pragma once

#include "common/pch.h"

class VideoFilter;
class Settings;

class Receiver : public QObject {
    Q_OBJECT

public:
    Receiver();
    ~Receiver() override;
    VideoFilter* get_video_filter() const;
    void set_camera(QCamera*);
    void start_receive(const std::unique_ptr<Settings>&);
    void stop_receive();

private:
    std::unique_ptr<VideoFilter> _video_filter;
    std::unique_ptr<QTimer> _timer;
    QCamera* _camera;
    QGeoPositionInfoSource* _geo_source;
};

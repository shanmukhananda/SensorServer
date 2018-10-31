#include "common/pch.h"

#include "common/settings.h"
#include "model/receiver.h"
#include "model/videofilter.h"

Receiver::Receiver()
    : _video_filter(std::make_unique<VideoFilter>())
    , _timer(nullptr)
    , _camera(nullptr)
    , _geo_source(nullptr) {
    _timer = std::make_unique<QTimer>(this);
    _geo_source = QGeoPositionInfoSource::createDefaultSource(this);
}

Receiver::~Receiver() {
}

VideoFilter* Receiver::get_video_filter() const {
    return _video_filter.get();
}

void Receiver::set_camera(QCamera* camera_) {
    _camera = camera_;
    _camera->focus()->setFocusMode(QCameraFocus::FocusMode::InfinityFocus);
}

void Receiver::start_receive(const std::unique_ptr<Settings>& settings_) {
    if (settings_->is_camera_enabled()) {
        QCameraViewfinderSettings vfsettings;
        vfsettings.setMinimumFrameRate(settings_->camera_frequency());
        vfsettings.setMaximumFrameRate(settings_->camera_frequency());
        QSize res;
        if (resolution::hd == settings_->image_resolution())
            res = QSize(1280, 720);
        else
            res = QSize(640, 480);
        vfsettings.setResolution(res);
        _camera->setViewfinderSettings(vfsettings);
        _camera->start();
    }

    else
        _camera->stop();

    if (settings_->is_gps_enabled())
        _geo_source->startUpdates();
    else
        _geo_source->stopUpdates();

    if (settings_->is_acceleromter_enabled() ||
        settings_->is_gyroscope_enabled())
        _timer->start(settings_->imu_frequency());
    else
        _timer->stop();
}

void Receiver::stop_receive() {
    _camera->stop();
    _geo_source->stopUpdates();
    _timer->stop();
}

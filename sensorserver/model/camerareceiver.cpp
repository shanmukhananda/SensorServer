
#include "sensorserver/common/pch.h"

#include "sensorserver/common/settings.h"
#include "sensorserver/model/camerareceiver.h"
#include "sensorserver/model/sensordata.h"

CameraReceiver::CameraReceiver() {
    LOG_SCOPE;
    init();
}

void CameraReceiver::init() {
    LOG_SCOPE;
    _video_probe = std::make_unique<QVideoProbe>(this);

    auto connected = false;
    connected =
        connect(_video_probe.get(), SIGNAL(videoFrameProbed(QVideoFrame)), this,
                SLOT(videoframe_probed(QVideoFrame)));
    Q_ASSERT(connected);
    Q_UNUSED(connected);
}

void CameraReceiver::process_videoframe(QVideoFrame* frame_) {
    // LOG_SCOPE;
    auto timestamp = QDateTime::currentMSecsSinceEpoch();

    Q_ASSERT(frame_->width() > 0);
    Q_ASSERT(frame_->height() > 0);
    Q_ASSERT(timestamp > 0);
    Q_ASSERT(frame_->mappedBytes() > 0);

    auto image = std::make_shared<QImage>(qt_imageFromVideoFrame(*frame_));

    if (QImage::Format::Format_Invalid == image->format())
        throw std::runtime_error("QVideoFrame to QImage Conversion Failed");

    emit received_image(timestamp, image);
}

CameraReceiver::~CameraReceiver() {
    LOG_SCOPE;
}

void CameraReceiver::update_camera(QCamera* camera_) {
    LOG_SCOPE;
    _camera = camera_;
    _camera->focus()->setFocusMode(QCameraFocus::FocusMode::InfinityFocus);
}

void CameraReceiver::start_reception(Settings* settings_) {
    LOG_SCOPE;
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
        _camera->setCaptureMode(QCamera::CaptureVideo);

        if (!_video_probe->setSource(_camera))
            throw std::runtime_error(
                "camera does not support monitoring video");
        _camera->start();
    } else
        _camera->stop();
}

void CameraReceiver::stop_reception() {
    LOG_SCOPE;
    _camera->stop();
}

void CameraReceiver::videoframe_probed(const QVideoFrame& vid_frame_) {
    // LOG_SCOPE;
    auto frame_ = std::make_unique<QVideoFrame>(vid_frame_);
    if (!frame_ || !frame_->isValid() ||
        !frame_->map(QAbstractVideoBuffer::ReadOnly))
        return;

    process_videoframe(frame_.get());
    frame_->unmap();
}

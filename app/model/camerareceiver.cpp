
#include "app/common/pch.h"

#include "app/common/settings.h"
#include "app/model/camerareceiver.h"
#include "app/model/sensordata.h"

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
            LOG_ERROR << "camera does not support monitoring video";
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

    auto timestamp = QDateTime::currentMSecsSinceEpoch();
    QString pix_format;
    QDebug(&pix_format) << frame_->pixelFormat();
    pix_format = pix_format.trimmed();

    Q_ASSERT(frame_->width() > 0);
    Q_ASSERT(frame_->height() > 0);
    Q_ASSERT(timestamp > 0);

    auto image_data = std::make_shared<ImageData>();
    image_data->timestamp = static_cast<std::uint64_t>(timestamp);
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

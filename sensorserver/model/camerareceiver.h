#pragma once

#include "sensorserver/common/pch.h"

class Settings;
class SensorData;

class CameraReceiver : public QObject {
    Q_OBJECT

public:
    CameraReceiver();
    ~CameraReceiver() override;
    void run();

signals:
    void received_image(qint64, std::shared_ptr<QImage>);

private slots:
    void update_camera(QCamera*);
    void start_reception(Settings*);
    void stop_reception();
    void videoframe_probed(const QVideoFrame&);

private:
    void init();
    void process_videoframe(QVideoFrame*);

    QCamera* _camera{nullptr};
    std::unique_ptr<QVideoProbe> _video_probe;
};

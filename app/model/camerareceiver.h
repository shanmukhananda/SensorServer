#pragma once

#include "app/common/pch.h"

class Settings;
class SensorData;

class CameraReceiver : public QObject {
    Q_OBJECT

public:
    CameraReceiver();
    ~CameraReceiver() override;
    void run();

signals:
    void received_sensordata(std::shared_ptr<SensorData>);

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

#pragma once

#include "app/common/pch.h"

class SensorData;

class Compressor : public QObject {
    Q_OBJECT
public:
    Compressor();
    ~Compressor() override;

signals:
    void received_sensordata(std::shared_ptr<SensorData>);

private slots:
    void received_image(qint64, std::shared_ptr<QImage>);
};

#pragma once

#include "sensorserver/common/pch.h"

class Settings;
class SensorData;

class GeoReceiver : public QObject {
    Q_OBJECT

public:
    GeoReceiver();
    ~GeoReceiver() override;

signals:
    void received_sensordata(std::shared_ptr<SensorData>);

public slots:
    void start_reception(Settings*);
    void stop_reception();
    void position_updated(const QGeoPositionInfo&);

private:
    void init();

    QGeoPositionInfoSource* _geo_source{nullptr};
};

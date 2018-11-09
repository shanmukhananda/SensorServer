#pragma once

#include "app/common/pch.h"

class SensorData;
class Settings;
class Sender;

class Transmitter : public QObject {
    Q_OBJECT

public:
    Transmitter();
    ~Transmitter() override;

signals:
    void status(const QString&);

private slots:
    void start_transmission(Settings*);
    void stop_transmission();
    void received_sensordata(std::shared_ptr<SensorData>);

private:
    void init();

    std::unique_ptr<Sender> _sender;
};

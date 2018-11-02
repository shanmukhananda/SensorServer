#pragma once

#include "common/pch.h"

class SensorData;
class Settings;

class Transmitter : public QObject {
    Q_OBJECT

public:
    Transmitter();
    ~Transmitter() override;

private slots:
    void start_transmission(const std::unique_ptr<Settings>&);
    void stop_transmission();
    void received_sensordata(std::shared_ptr<SensorData>);
    void new_connection();
    void client_disconnected();

private:
    std::unique_ptr<QTcpServer> _tcp_server;
    QTcpSocket* _socket{nullptr};
};

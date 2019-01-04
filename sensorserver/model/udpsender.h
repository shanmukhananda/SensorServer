#pragma once

#include "sensorserver/common/pch.h"

#include "sensorserver/model/sender.h"

class SensorData;
class Settings;

class UDPSender : public Sender {
    Q_OBJECT

public:
    explicit UDPSender(QObject* = nullptr);
    ~UDPSender() override;
    void send(std::shared_ptr<SensorData>) override;
    void start_transmission(Settings*) override;
    void stop_transmission() override;

signals:
    void status(const QString&);

private:
    void update_status(const QString&);

    QUdpSocket* _socket{nullptr};
    QString _status;
    std::mutex _socket_lock;
    std::pair<QHostAddress, quint16> _ip_port;
};

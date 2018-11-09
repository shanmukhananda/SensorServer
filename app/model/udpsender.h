#pragma once

#include "app/common/pch.h"
#include "app/model/sender.h"

class SensorData;
class Settings;

// struct UDPHeader {
//     std::uint8_t type = 0;
//     std::uint64_t timestamp = 0;
//     std::uint64_t sequence_number = 0;
//     std::uint64_t payload_size = 0;
// };

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

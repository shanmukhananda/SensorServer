#pragma once

#include "common/pch.h"

class SensorData;
class Settings;

class Transmitter : public QObject {
    Q_OBJECT

public:
    Transmitter();
    ~Transmitter() override;

signals:
    void status(const QString&);

private slots:
    void start_transmission(const std::unique_ptr<Settings>&);
    void stop_transmission();
    void received_sensordata(std::shared_ptr<SensorData>);
    void new_connection();
    void client_disconnected();

private:
    std::pair<QHostAddress, quint16>
    get_ip_and_port(const std::unique_ptr<Settings>&);
    void create_server(const std::unique_ptr<Settings>&);
    void close_server();
    void socket_create();
    bool socket_write(const std::shared_ptr<SensorData>&);
    void update_status(const QString&);

    QTcpServer* _tcp_server{nullptr};
    QTcpSocket* _socket{nullptr};
    QString _status;
};

#pragma once

#include "app/common/pch.h"

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
    void start_transmission(Settings*);
    void stop_transmission();
    void received_sensordata(std::shared_ptr<SensorData>);
    void new_connection();
    void client_disconnected();

private:
    std::pair<QHostAddress, quint16> get_ip_and_port(Settings*);
    void create_server(Settings*);
    void close_server();
    void socket_create();
    bool socket_write(std::shared_ptr<SensorData>);
    void update_status(const QString&);

    QTcpServer* _tcp_server{nullptr};
    QTcpSocket* _socket{nullptr};
    QString _status;
};

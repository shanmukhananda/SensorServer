#pragma once

#include "app/common/pch.h"
#include "app/model/sender.h"

class SensorData;
class Settings;

class TCPSender : public Sender {
    Q_OBJECT

public:
    explicit TCPSender(QObject* = nullptr);
    ~TCPSender() override;
    void send(std::shared_ptr<SensorData>) override;
    void start_transmission(Settings*) override;
    void stop_transmission() override;

signals:
    void status(const QString&);

public slots:
    void new_connection();
    void client_disconnected();

private:
    void create_server(Settings*);
    void close_server();
    void send_impl(std::shared_ptr<SensorData>);
    void socket_create();
    bool socket_write(std::shared_ptr<SensorData>);
    void update_status(const QString&);

    QTcpServer* _tcp_server{nullptr};
    QTcpSocket* _socket{nullptr};
    QString _status;
    std::mutex _socket_lock;
};

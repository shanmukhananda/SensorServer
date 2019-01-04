#include "sensorserver/common/pch.h"

#include "sensorserver/common/settings.h"
#include "sensorserver/model/sensordata.h"
#include "sensorserver/model/tcpsender.h"

TCPSender::TCPSender(QObject* parent_) : Sender(parent_) {
    LOG_SCOPE;
}

TCPSender::~TCPSender() {
    LOG_SCOPE;
}

void TCPSender::create_server(Settings* settings_) {
    LOG_SCOPE;
    close_server();
    _tcp_server = new QTcpServer(this);

    auto connected = false;
    connected = connect(_tcp_server, SIGNAL(newConnection()), this,
                        SLOT(new_connection()));
    Q_ASSERT(connected);
    Q_UNUSED(connected);

    auto ip_port = settings_->get_ip_and_port();
    if (!_tcp_server->listen(ip_port.first, ip_port.second))
        throw std::runtime_error("server could not start");
    _tcp_server->setMaxPendingConnections(1);
}

void TCPSender::close_server() {
    LOG_SCOPE;
    if (_tcp_server) {
        _tcp_server->close();
        _tcp_server->deleteLater();
        _tcp_server = nullptr;
    }
}

void TCPSender::socket_create() {
    LOG_SCOPE;
    std::lock_guard<std::mutex> locker(_socket_lock);
    if (_socket && _socket->isValid()) {
        LOG_DEBUG << "server busy";
        _tcp_server->nextPendingConnection();
        return;
    }

    _socket = _tcp_server->nextPendingConnection();

    if (!_socket)
        throw std::runtime_error("unable to create socket");

    auto connected = false;
    connected = connect(_socket, SIGNAL(disconnected()), this,
                        SLOT(client_disconnected()));
    Q_ASSERT(connected);
    Q_UNUSED(connected);
}

bool TCPSender::socket_write(std::shared_ptr<SensorData> sensor_data_) {
    // LOG_SCOPE;
    std::lock_guard<std::mutex> locker(_socket_lock);
    if (!_socket || !_socket->isValid())
        return false;

    auto data_ = sensor_data_->serialize();
    auto data = reinterpret_cast<const char*>(data_.data());
    auto size = static_cast<qint64>(data_.size());

    decltype(size) total_bytes_written = 0;

    while (total_bytes_written < size) {
        auto bytes_written = _socket->write(data + total_bytes_written,
                                            size - total_bytes_written);
        if (bytes_written < 0)
            throw std::runtime_error("socket write failed");

        total_bytes_written += bytes_written;
        _socket->flush();
        _socket->waitForBytesWritten();
    }

    return true;
}

void TCPSender::update_status(const QString& current_status_) {
    if (current_status_ != _status) {
        _status = current_status_;
        LOG_DEBUG << "updating status to " << _status;
        emit status(_status);
    }
}

void TCPSender::stop_transmission() {
    LOG_SCOPE;
    close_server();
}

void TCPSender::client_disconnected() {
    LOG_SCOPE;
    std::lock_guard<std::mutex> locker(_socket_lock);
    _socket->deleteLater();
    _socket = nullptr;
}

void TCPSender::start_transmission(Settings* settings_) {
    LOG_SCOPE;

    QString current_status;
    try {
        create_server(settings_);
        current_status = "ok";
    } catch (std::exception& e) {
        LOG_ERROR << "caught exception:" << e.what();
        current_status = e.what();

    } catch (...) {
        LOG_ERROR << "caught unknown exception";
        current_status = "unknown error";
    }

    update_status(current_status);
}

void TCPSender::send(std::shared_ptr<SensorData> data_) {
    QString current_status{"not connected"};
    try {
        if (socket_write(data_))
            current_status = "ok";
    } catch (std::exception& e) {
        LOG_ERROR << "caught exception:" << e.what();
        current_status = e.what();
    } catch (...) {
        LOG_ERROR << "caught unknown exception";
        current_status = "unknown error";
    }

    update_status(current_status);
}

void TCPSender::new_connection() {
    LOG_SCOPE;

    QString current_status;
    try {
        socket_create();
        current_status = "ok";
    } catch (std::exception& e) {
        LOG_ERROR << "caught exception:" << e.what();
        current_status = e.what();
    } catch (...) {
        LOG_ERROR << "caught unknown exception";
        current_status = "unknown error";
    }

    update_status(current_status);
}

#include "app/common/pch.h"

#include "app/common/settings.h"
#include "app/model/sensordata.h"
#include "app/model/transmitter.h"

Transmitter::Transmitter() {
    LOG_SCOPE;
}

Transmitter::~Transmitter() {
    LOG_SCOPE;
}

void Transmitter::create_server(Settings* settings_) {
    LOG_SCOPE;
    _tcp_server = new QTcpServer(this);

    auto connected = false;
    connected = connect(_tcp_server, SIGNAL(newConnection()), this,
                        SLOT(new_connection()));
    Q_ASSERT(connected);
    Q_UNUSED(connected);

    auto ip_port = get_ip_and_port(settings_);
    if (!_tcp_server->listen(ip_port.first, ip_port.second))
        throw std::runtime_error("server could not start");
    _tcp_server->setMaxPendingConnections(1);
}

void Transmitter::close_server() {
    LOG_SCOPE;
    if (_tcp_server) {
        _tcp_server->close();
        _tcp_server->deleteLater();
        _tcp_server = nullptr;
    }
}

void Transmitter::socket_create() {
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

bool Transmitter::socket_write(const std::shared_ptr<SensorData>& sensor_data_) {
    if (!_socket || !_socket->isValid())
        return false;

    auto buffer = sensor_data_->serialize();
    _socket->write(reinterpret_cast<const char*>(buffer.data()),
                   static_cast<qint64>(buffer.size()));
    _socket->flush();
    _socket->waitForBytesWritten();

    return true;
}

void Transmitter::update_status(const QString& current_status_) {
    if (current_status_ != _status) {
        _status = current_status_;
        LOG_DEBUG << "updating status to " << _status;
        emit status(_status);
    }
}

void Transmitter::stop_transmission() {
    LOG_SCOPE;
    close_server();
}

void Transmitter::client_disconnected() {
    LOG_SCOPE;
    _socket->deleteLater();
    _socket = nullptr;
}

std::pair<QHostAddress, quint16>
Transmitter::get_ip_and_port(Settings* settings_) {
    QHostAddress ip_add;
    if (!ip_add.setAddress(settings_->ip()))
        throw std::runtime_error("invalid ip address");

    auto ok = false;
    int port_temp = settings_->port().toInt(&ok);

    if (port_temp <= 0 && !ok)
        throw std::runtime_error("invalid port");

    auto port = static_cast<quint16>(port_temp);
    return std::make_pair(ip_add, port);
}

void Transmitter::start_transmission(Settings* settings_) {
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

void Transmitter::received_sensordata(std::shared_ptr<SensorData> sensor_data_) {
    // LOG_SCOPE;

    QString current_status{"not connected"};
    try {
        if (socket_write(sensor_data_))
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

void Transmitter::new_connection() {
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

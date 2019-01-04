#include "sensorserver/common/pch.h"

#include "sensorserver/common/settings.h"
#include "sensorserver/model/sensordata.h"
#include "sensorserver/model/wssender.h"

WSSender::WSSender(QObject* parent_) : Sender(parent_) {
    LOG_SCOPE;
}

WSSender::~WSSender() {
    LOG_SCOPE;
}

void WSSender::create_server(Settings* settings_) {
    LOG_SCOPE;
    close_server();
    _ws_server = new QWebSocketServer(QStringLiteral("SensorServer"),
                                      QWebSocketServer::NonSecureMode, this);

    auto connected = false;
    connected = connect(_ws_server, SIGNAL(newConnection()), this,
                        SLOT(new_connection()));
    Q_ASSERT(connected);
    Q_UNUSED(connected);

    auto ip_port = settings_->get_ip_and_port();
    if (!_ws_server->listen(ip_port.first, ip_port.second))
        throw std::runtime_error("server could not start");
    _ws_server->setMaxPendingConnections(1);
}

void WSSender::close_server() {
    LOG_SCOPE;
    if (_ws_server) {
        _ws_server->close();
        _ws_server->deleteLater();
        _ws_server = nullptr;
    }
}

void WSSender::socket_create() {
    LOG_SCOPE;
    std::lock_guard<std::mutex> locker(_socket_lock);
    if (_socket && _socket->isValid()) {
        LOG_DEBUG << "server busy";
        _ws_server->nextPendingConnection();
        return;
    }

    _socket = _ws_server->nextPendingConnection();

    if (!_socket)
        throw std::runtime_error("unable to create socket");

    auto connected = false;
    connected = connect(_socket, SIGNAL(disconnected()), this,
                        SLOT(client_disconnected()));
    Q_ASSERT(connected);
    Q_UNUSED(connected);
}

bool WSSender::socket_write(std::shared_ptr<SensorData> sensor_data_) {
    // LOG_SCOPE;
    std::lock_guard<std::mutex> locker(_socket_lock);
    if (!_socket || !_socket->isValid())
        return false;

    _socket->sendBinaryMessage(sensor_data_->serialize());
    _socket->flush();

    return true;
}

void WSSender::update_status(const QString& current_status_) {
    if (current_status_ != _status) {
        _status = current_status_;
        LOG_DEBUG << "updating status to " << _status;
        emit status(_status);
    }
}

void WSSender::stop_transmission() {
    LOG_SCOPE;
    close_server();
}

void WSSender::client_disconnected() {
    LOG_SCOPE;
    std::lock_guard<std::mutex> locker(_socket_lock);
    _socket->deleteLater();
    _socket = nullptr;
}

void WSSender::start_transmission(Settings* settings_) {
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

void WSSender::send(std::shared_ptr<SensorData> data_) {
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

void WSSender::new_connection() {
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

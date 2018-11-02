#include "common/pch.h"

#include "model/sensordata.h"
#include "model/transmitter.h"

Transmitter::Transmitter() {
    _tcp_server = std::make_unique<QTcpServer>(this);

    auto connected = false;
    connected = connect(_tcp_server.get(), SIGNAL(newConnection()), this,
                        SLOT(new_connection()));
    Q_ASSERT(connected);

    Q_UNUSED(connected);
}

Transmitter::~Transmitter() {
}

void Transmitter::start_transmission(const std::unique_ptr<Settings>&) {
#if 0
    if (_tcp_server->isListening()) {
        _tcp_server->close();
        _queue.clear();
    }

    QHostAddress ip_add;
    if (!ip_add.setAddress(ip_)) {
        auto msg = "invalid ip address";
        qDebug() << msg;
        // todo emit status(msg);
        return;
    }

    bool ok;
    int port_temp = port_.toInt(&ok);

    if (port_temp <= 0 && !ok) {
        auto msg = "invalid port";
        qDebug() << msg;
        // todo emit status(msg);
        return;
    }

    auto port = static_cast<quint16>(port_temp);

    if (!_tcp_server->listen(ip_add, port)) {
        auto msg = "server could not start";
        qDebug() << msg;
        // todo emit status(msg);
        return;
    }
#endif
}

void Transmitter::stop_transmission() {
}

void Transmitter::received_sensordata(std::shared_ptr<SensorData> sensor_data_) {
    qDebug() << QThread::currentThreadId() << __FUNCTION__;
    Q_UNUSED(sensor_data_);

#if 0
    if (!_socket)
        continue;

    QString msg = QString::number(item->timestamp) + " " +
                  QString::number(static_cast<uint8_t>(item->type));
    qDebug() << QThread::currentThreadId() << msg;
    _socket->write(msg.toStdString().c_str());
    _socket->flush();
    _socket->waitForBytesWritten();
#endif
}

void Transmitter::new_connection() {
    qDebug() << QThread::currentThreadId() << __FUNCTION__;
#if 0
    _socket = _tcp_server->nextPendingConnection();
    auto connected = false;
    connected = connect(_socket, SIGNAL(disconnected()), this,
                        SLOT(client_disconnected()));

    Q_ASSERT(connected);

    Q_UNUSED(connected);
#endif
}

void Transmitter::client_disconnected() {
    qDebug() << QThread::currentThreadId() << __FUNCTION__;
    _socket->close();
    delete _socket;
    _socket = nullptr;
}

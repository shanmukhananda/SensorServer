#include "app/common/pch.h"

#include "app/common/settings.h"
#include "app/messages/udpheader_generated.h"
#include "app/model/sensordata.h"
#include "app/model/udpsender.h"

UDPSender::UDPSender(QObject* parent) : Sender(parent) {
    LOG_SCOPE;
}

UDPSender::~UDPSender() {
    LOG_SCOPE;
}

void UDPSender::send(std::shared_ptr<SensorData> sensor_data_) {
    // LOG_SCOPE;

    QString current_status{"not broadcasting"};
    try {
        if (!_socket)
            return;

        current_status = "ok";
        auto data_ = sensor_data_->serialize();

        constexpr int dgram_size = 1024;
        constexpr int bunch_size = dgram_size - sizeof(messages::UDPHeader);

        for (decltype(data_.size()) i = 0; i < data_.size(); i += bunch_size) {
            auto header = messages::UDPHeader{
                static_cast<std::uint8_t>(sensor_data_->type()),
                sensor_data_->timestamp(), i, data_.size()};

            auto bunch_end = std::min<int>(data_.size(), i + bunch_size);
            auto begin_iter = data_.begin() + i;
            auto end_iter = data_.begin() + bunch_end;
            auto current_bunch_size = std::distance(begin_iter, end_iter);

            QByteArray dgram;
            dgram.reserve(dgram_size);

            auto header_start = reinterpret_cast<const char*>(&header);
            auto header_len = static_cast<int>(sizeof(header));
            dgram.append(header_start, header_len);
            auto dgram_start = reinterpret_cast<const char*>(&(*begin_iter));
            auto dgram_len = current_bunch_size;
            dgram.append(dgram_start, dgram_len);
            _socket->writeDatagram(dgram, _ip_port.first, _ip_port.second);
        }
    } catch (std::exception& e) {
        LOG_ERROR << "caught exception:" << e.what();
        current_status = e.what();
    } catch (...) {
        LOG_ERROR << "caught unknown exception";
        current_status = "unknown error";
    }

    update_status(current_status);
}

void UDPSender::start_transmission(Settings* settings_) {
    LOG_SCOPE;
    QString current_status{"not broadcasting"};
    try {
        stop_transmission();
        _ip_port = settings_->get_ip_and_port();
        _socket = new QUdpSocket(this);
    } catch (std::exception& e) {
        LOG_ERROR << "caught exception:" << e.what();
        current_status = e.what();
    } catch (...) {
        LOG_ERROR << "caught unknown exception";
        current_status = "unknown error";
    }

    update_status(current_status);
}

void UDPSender::stop_transmission() {
    LOG_SCOPE;
    if (_socket) {
        _socket->close();
        _socket->deleteLater();
        _socket = nullptr;
    }
}

void UDPSender::update_status(const QString& current_status_) {
    if (current_status_ != _status) {
        _status = current_status_;
        LOG_DEBUG << "updating status to " << _status;
        emit status(_status);
    }
}

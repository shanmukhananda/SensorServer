#include "sensorserver/common/pch.h"

#include "sensorserver/common/settings.h"
#include "sensorserver/model/sensordata.h"
#include "sensorserver/model/tcpsender.h"
#include "sensorserver/model/transmitter.h"
#include "sensorserver/model/udpsender.h"
#include "sensorserver/model/wssender.h"

Transmitter::Transmitter() {
    LOG_SCOPE;
    init();
}

void Transmitter::init() {
    LOG_SCOPE;
    _sender = std::make_unique<WSSender>(this);
    auto connected = false;
    connected = connect(_sender.get(), SIGNAL(status(QString)), this,
                        SIGNAL(status(QString)));
    Q_ASSERT(connected);
    Q_UNUSED(connected);
}

Transmitter::~Transmitter() {
    LOG_SCOPE;
}

void Transmitter::stop_transmission() {
    LOG_SCOPE;
    _sender->stop_transmission();
}

void Transmitter::start_transmission(Settings* settings_) {
    LOG_SCOPE;
    _sender->start_transmission(settings_);
}

void Transmitter::received_sensordata(std::shared_ptr<SensorData> sensor_data_) {
    // LOG_SCOPE;
    _sender->send(sensor_data_);
}

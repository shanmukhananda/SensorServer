#pragma once

#include "app/common/pch.h"

class SensorData;
class Settings;

class Sender : public QObject {
    Q_OBJECT
public:
    explicit Sender(QObject* parent = nullptr) : QObject(parent) {
    }
    virtual ~Sender() = default;
    virtual void send(std::shared_ptr<SensorData>) = 0;
    virtual void start_transmission(Settings*) = 0;
    virtual void stop_transmission() = 0;

signals:
    void status(const QString&);
};

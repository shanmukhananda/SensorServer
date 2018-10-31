#pragma once

#include "common/pch.h"

#include "model/videofilter.h"

class Receiver;
class Settings;

class Model : public QObject {
    Q_OBJECT

public:
    Model();
    ~Model() override;
    void start();

signals:
    void model_initalized(QObject*, Settings*);

private slots:
    void view_initialized(QCamera*);
    void started(QString, QString);
    void stopped();
    void resolution_vga();
    void resolution_hd();
    void accelerometer_toggled(bool);
    void gyroscope_toggled(bool);
    void gps_toggled(bool);
    void camera_toggled(bool);

private:
    std::unique_ptr<Receiver> _receiver;
    std::unique_ptr<Settings> _settings;
};

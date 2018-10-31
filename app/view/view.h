#pragma once

#include "common/pch.h"

namespace Ui {
class View;
}

class Presenter;
class Settings;

class View : public QMainWindow {
    Q_OBJECT

public:
    View();
    ~View() override;
    void start();

signals:
    void view_initialized(QCamera*);
    void started(QString, QString);
    void stopped();
    void resolution_vga();
    void resolution_hd();
    void accelerometer_toggled(bool);
    void gyroscope_toggled(bool);
    void gps_toggled(bool);
    void camera_toggled(bool);

private slots:
    void on_pushButton_start_stop_toggled(bool);
    void on_checkBox_accelerometer_stateChanged(int);
    void on_checkBox_camera_stateChanged(int);
    void on_checkBox_gyroscope_stateChanged(int);
    void on_checkBox_gps_stateChanged(int);
    void on_radioButton_1280_x_720_toggled(bool);
    void on_radioButton_640_x_480_toggled(bool);
    void model_initalized(QObject*, Settings*);

private:
    void enable_controls(bool);
    void update_start_stop_button(QColor, QString);
    void update_view(Settings*);

    Ui::View* ui;
};

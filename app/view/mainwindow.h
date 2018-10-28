#pragma once

#include "common/pch.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_start_stop_toggled(bool);
    void on_checkBox_accelerometer_stateChanged(int);
    void on_checkBox_camera_stateChanged(int);
    void on_checkBox_gyroscope_stateChanged(int);
    void on_checkBox_gps_stateChanged(int);
    void on_radioButton_1280_x_720_toggled(bool);
    void on_radioButton_640_x_480_toggled(bool);

private:
    void enabled_controls(bool);
    Ui::MainWindow* ui;
    QCamera* _camera{nullptr};
    QCameraViewfinderSettings _camera_settings;

    enum class resolution {
        vga,
        hd
    };

    resolution _resolution;
};

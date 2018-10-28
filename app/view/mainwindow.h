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
    void on_pushButton_start_stop_toggled(bool checked);
    void on_checkBox_accelerometer_stateChanged(int arg1);
    void on_checkBox_camera_stateChanged(int arg1);
    void on_checkBox_gyroscope_stateChanged(int arg1);
    void on_checkBox_gps_stateChanged(int arg1);
    void on_radioButton_1280_x_720_toggled(bool checked);
    void on_radioButton_640_x_480_toggled(bool checked);

private:
    void enabled_controls(bool);
    Ui::MainWindow* ui;
};

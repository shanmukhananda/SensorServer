#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class QAccelerometer;
class QGyroscope;
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void print();

private:
    Ui::MainWindow* ui;
    QAccelerometer* accel;
    QGyroscope* gyro;
};

#endif // MAINWINDOW_H

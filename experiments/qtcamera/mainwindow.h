#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QBuffer>
#include <QByteArray>
#include <QCamera>
#include <QCameraImageCapture>
#include <QCameraInfo>
#include <QDateTime>
#include <QDebug>
#include <QMainWindow>
#include <QVideoProbe>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void init();

private slots:
    void imageCaptured(int id, QImage img);

    void readyForCaptureChanged(bool state);
    void videoFrameProbed(QVideoFrame);

private:
    Ui::MainWindow* ui;
    QCamera* camera{nullptr};
    QCameraImageCapture* cap{nullptr};
};

#endif // MAINWINDOW_H

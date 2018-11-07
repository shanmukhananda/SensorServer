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
    void videoFrameProbed(const QVideoFrame&);

private:
    Ui::MainWindow* ui;
    QCamera* camera{nullptr};
    QVideoProbe* videoProbe{nullptr};
};

#endif // MAINWINDOW_H

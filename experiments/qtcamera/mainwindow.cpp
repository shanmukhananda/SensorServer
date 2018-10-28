#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::imageCaptured(int id, QImage img) {
    qDebug() << __FUNCTION__;
    return;
    QByteArray buf;
    QBuffer buffer(&buf);
    buffer.open(QIODevice::WriteOnly);
    static std::size_t count = 0;
    QString fname = QString::number(count++) + ".jpeg";
    qDebug() << QDateTime::currentMSecsSinceEpoch() << ":ID=" << id
             << ":Saving to " << fname;
    img.save(fname, "JPEG");
}

void MainWindow::readyForCaptureChanged(bool state) {
    qDebug() << __FUNCTION__;
    return;
    if (state == true) {
        camera->searchAndLock();
        cap->capture();
        camera->unlock();
    }
}

void MainWindow::videoFrameProbed(QVideoFrame) {
    qDebug() << __FUNCTION__;
}

void MainWindow::init() {
    qDebug() << "initializing camera";
    camera = new QCamera(QCamera::FrontFace, this);
    camera->setCaptureMode(QCamera::CaptureVideo);
    cap = new QCameraImageCapture(camera);

    QImageEncoderSettings imageSettings;
    imageSettings.setCodec("image/jpeg");
    imageSettings.setResolution(640, 480);
    imageSettings.setQuality(QMultimedia::VeryHighQuality);
    cap->setEncodingSettings(imageSettings);

    cap->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
    QObject::connect(cap, SIGNAL(imageCaptured(int, QImage)), this,
                     SLOT(imageCaptured(int, QImage)));
    QObject::connect(cap, SIGNAL(readyForCaptureChanged(bool)), this,
                     SLOT(readyForCaptureChanged(bool)));
    auto videoProbe = new QVideoProbe(this);

    if (videoProbe->setSource(camera)) {
        // Probing succeeded, videoProbe->isValid() should be true.
        connect(videoProbe, SIGNAL(videoFrameProbed(QVideoFrame)), this,
                SLOT(videoFrameProbed(QVideoFrame)));
    }

    qDebug() << "Starting camera";
    camera->start();
}

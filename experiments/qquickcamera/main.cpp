//#include "mainwindow.h"
#include <QAbstractVideoFilter>
#include <QApplication>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QVideoFilterRunnable>
#include <QtQml>

#include <QCamera>
#include <QCameraImageCapture>
#include <QImageEncoderSettings>
#include <exception>

class VideoFilterRunnable : public QVideoFilterRunnable {
public:
    QVideoFrame
    run(QVideoFrame* input, const QVideoSurfaceFormat& /*surfaceFormat*/,
        RunFlags /*flags*/) override;
};

class VideoFilter : public QAbstractVideoFilter {
public:
    QVideoFilterRunnable* createFilterRunnable();
signals:
    void finished(QObject* result);
};

QImage imageFromVideoFrame(const QVideoFrame& buffer) {
    QImage img;
    QVideoFrame frame(buffer); // make a copy we can call map (non-const) on
    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage::Format imageFormat =
        QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat());
    // BUT the frame.pixelFormat() is QVideoFrame::Format_Jpeg, and this is
    // mapped to QImage::Format_Invalid by
    // QVideoFrame::imageFormatFromPixelFormat
    if (imageFormat != QImage::Format_Invalid) {
        img = QImage(frame.bits(), frame.width(), frame.height(),
                     // frame.bytesPerLine(),
                     imageFormat);
    } else {
        // e.g. JPEG
        auto nbytes = frame.mappedBytes();
        img = QImage::fromData(frame.bits(), nbytes);
    }
    frame.unmap();
    return img;
}

QVideoFrame
VideoFilterRunnable::run(QVideoFrame* input, const QVideoSurfaceFormat&,
                         QVideoFilterRunnable::RunFlags) {
    auto image = imageFromVideoFrame(*input);
    static int count = 0;
    auto fname = QString::number(count++) + ".jpeg";
    qDebug() << "Saving " << fname;
    //    image.save(fname, "JPEG");
    return *input;
}

QVideoFilterRunnable* VideoFilter::createFilterRunnable() {
    return new VideoFilterRunnable();
}

int main(int argc, char** argv) {
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    qmlRegisterType<VideoFilter>("QQuickCamera", 1, 0, "VideoFilter");
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
